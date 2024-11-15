#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <xaudio2.h>
#include <wrl/client.h>

struct SoundData{
	WAVEFORMATEX wfex;
	BYTE* pBuffer;
    unsigned int size;
};

class Audio{
	struct ChunkHeader{
		char id[4];
		int32_t size;
	};

	struct RiffHeader{
		ChunkHeader chunk;
		char type[4];
	};

	struct FormatChunk{
		ChunkHeader chunk;
        WAVEFORMATEX format;
	};

    Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
    IXAudio2MasteringVoice* masteringVoice_ = nullptr;

    std::unordered_map<std::string, SoundData> loaded_;

	uint32_t playingAudioCount_ = 0;
	std::unordered_map<uint32_t, IXAudio2SourceVoice*> playing_;

    std::string folderPath_ = "Assets/Sound/";
private:
	static std::shared_ptr<Audio> instance_;

	Audio() = default;
	~Audio();

private:
	void LoadWave(const std::string& fileName);

public:
	Audio(const Audio&) = delete;
    Audio& operator=(const Audio&) = delete;

    static std::shared_ptr<Audio> GetInstance();

public:
	void Initialize();

	void Load(const std::string& fileName);
    void Unload(const std::string& name);

	uint32_t Play(const SoundData& soundData);
	void Stop(uint32_t handle);

	//TODO
	//Volume
	//Loopable
	//Pause
	//Resume
	//Effects
};

