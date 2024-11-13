#pragma once
#include <cstdint>
#include <string>
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

public:
	~Audio();
	void Initialize();

	SoundData Load(const std::string& fileName);
    void Unload(SoundData& soundData);

    void Play(const SoundData& soundData) const;
};

