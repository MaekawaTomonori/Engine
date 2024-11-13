#include "Audio.h"

#include <cassert>
#include <fstream>

#pragma comment(lib, "xaudio2.lib")

Audio::~Audio() {
    xAudio2_.Reset();
}

void Audio::Initialize() {
    HRESULT hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
    assert(SUCCEEDED(hr));

    hr = xAudio2_->CreateMasteringVoice(&masteringVoice_);
    assert(SUCCEEDED(hr));


}

SoundData Audio::Load(const std::string& fileName) {
    std::ifstream file;
    file.open(fileName.c_str(), std::ios_base::binary);
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

    return soundData;
}

void Audio::Unload(SoundData& soundData) {
    delete[] soundData.pBuffer;

    soundData.pBuffer = nullptr;
    soundData.size = 0;
    soundData.wfex = {};
}

void Audio::Play(const SoundData& soundData) const {
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
}
