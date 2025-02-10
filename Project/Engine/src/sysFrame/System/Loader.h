#pragma once
#include <string>

#include "Sound/Audio.h"

namespace Loader{
	enum class Type{
		TEXTURE,
        MODEL,
        AUDIO
	};

    void Texture(const std::string& path);
    void Model(const std::string& path);
    AudioManager::SoundHandle Audio(const std::string& path);

    void Unload(Type type);
};



