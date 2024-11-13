#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
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

	uint32_t playingAudioCount_ = 0;
	std::unordered_map<uint32_t, IXAudio2SourceVoice*> playingAudios_;
public:
	~Audio();
	void Initialize();

	SoundData Load(const std::string& fileName);
    void Unload(SoundData& soundData);

	uint32_t Play(const SoundData& soundData);
	void Stop(uint32_t handle);

	//TODO
	//Volume
	//Loopable
	//Pause
	//Resume
	//Effects
};

