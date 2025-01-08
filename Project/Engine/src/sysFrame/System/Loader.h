#pragma once
#include <string>

namespace Loader{
	enum class Type{
		TEXTURE,
        MODEL,
        AUDIO
	};

    void Texture(const std::string& path);
    void Model(const std::string& path);
    void Audio(const std::string& path);

    void Unload(Type type);
};



