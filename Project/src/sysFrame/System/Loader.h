#pragma once
#include <deque>
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

    std::deque<std::string> LogFile(const std::string& path);

    void Unload(Type type);
};



