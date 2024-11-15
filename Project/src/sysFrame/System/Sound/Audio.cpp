#include "Audio.h"

#include <cassert>
#include <fstream>

#pragma comment(lib, "xaudio2.lib")

std::shared_ptr<Audio> Audio::instance_ = nullptr;

Audio::~Audio() {
    xAudio2_.Reset();
}

std::shared_ptr<Audio> Audio::GetInstance() {
    if (!instance_){
        instance_ = std::shared_ptr<Audio>(new Audio, [](const Audio* ptr){
            delete ptr;
        });
    }

    return instance_;
}

void Audio::Initialize() {
    HRESULT hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
    assert(SUCCEEDED(hr));

    hr = xAudio2_->CreateMasteringVoice(&masteringVoice_);
    assert(SUCCEEDED(hr));


}

void Audio::Load(const std::string& fileName) {
    std::string name;
    size_t pos = fileName.find_last_of('/');
    if (pos != std::string::npos){
        name = fileName.substr(pos + 1);
    } else{
        name = fileName;
    }

    if (loaded_.contains(name))return;

    LoadWave(name);
}

void Audio::LoadWave(const std::string& fileName) {
    std::string name = folderPath_ + fileName;

    std::ifstream file;
    file.open(name.c_str(), std::ios_base::binary);
	assert(file.is_open());

    RiffHeader riff;
    file.read(reinterpret_cast<char*>(&riff), sizeof(riff));

    if(strncmp(riff.chunk.id, "RIFF", 4) != 0){
        assert(false);
    }
    if (strncmp(riff.type, "WAVE", 4) != 0){
        assert(false);
    }

    FormatChunk format = {};
    file.read(reinterpret_cast<char*>(&format), sizeof(ChunkHeader));
    if (strncmp(format.chunk.id, "fmt ", 4) != 0){
        assert(false);
    }

    assert(format.chunk.size <= sizeof(format.format));
    file.read(reinterpret_cast<char*>(&format.format), format.chunk.size);

    ChunkHeader data{};
    file.read(reinterpret_cast<char*>(&data), sizeof(data));
    if (strncmp(data.id, "JUNK", 4) == 0){
        file.seekg(data.size, std::ios_base::cur);
        file.read(reinterpret_cast<char*>(&data), sizeof(data));
    }

	if (strncmp(data.id, "data", 4) != 0){
        assert(false);
    }

    char* pBuffer = new char[data.size];
    file.read(pBuffer, data.size);

    file.close();

    SoundData soundData = {};
    soundData.wfex = format.format;
    soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
    soundData.size = data.size;

    loaded_.emplace(name, soundData);
}

void Audio::Unload(const std::string& name) {
    if (!loaded_.contains(name))return;

    SoundData& soundData = loaded_.at(name);

    delete[] soundData.pBuffer;

    soundData.pBuffer = nullptr;
    soundData.size = 0;
    soundData.wfex = {};

    loaded_.erase(name);
}

uint32_t Audio::Play(const SoundData& soundData) {
    HRESULT hr = S_OK;

    IXAudio2SourceVoice* pSourceVoice = nullptr;
    hr = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
    assert(SUCCEEDED(hr));

    XAUDIO2_BUFFER buffer = {};
    buffer.pAudioData = soundData.pBuffer;
    buffer.AudioBytes = soundData.size;
    buffer.Flags = XAUDIO2_END_OF_STREAM;

    hr = pSourceVoice->SubmitSourceBuffer(&buffer);
    assert(SUCCEEDED(hr));

    hr = pSourceVoice->Start();
    assert(SUCCEEDED(hr));

    playing_.emplace(playingAudioCount_, pSourceVoice);
    return playingAudioCount_++;
}

void Audio::Stop(const uint32_t handle) {
    if (!playing_.contains(handle))return;

    playing_.at(handle)->Stop();
    playing_.at(handle)->DestroyVoice();
    playing_.erase(handle);
}
