#include "Loader.h"

#include "DirectX/Model/ModelManager.h"
#include "DirectX/Texture/TextureManager.h"
#include "Sound/Audio.h"

void Loader::Texture(const std::string& path) {
    TextureManager::GetInstance()->Load(path);
}

void Loader::Model(const std::string& path) {
    ModelManager::GetInstance()->Load(path);
}

AudioManager::SoundHandle Loader::Audio(const std::string& path) {
    return AudioManager::GetInstance()->Load(path);
}

void Loader::Unload(Type type) {
    if (type != Type::TEXTURE)return;

    TextureManager::GetInstance()->Unload();
}
