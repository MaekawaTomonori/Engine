#include "Loader.h"

#include <fstream>
#include <future>

#include "DirectX/Model/ModelManager.h"
#include "DirectX/Texture/TextureManager.h"
#include "Sound/Audio.h"

void Loader::Texture(const std::string& path) {
    TextureManager::GetInstance()->Load(path);
}

void Loader::Model(const std::string& path) {
    auto future = std::async(std::launch::async, [&](){
        ModelManager::GetInstance()->Load(path);
    });

    future.get();
}

AudioManager::SoundHandle Loader::Audio(const std::string& path) {
    return AudioManager::GetInstance()->Load(path);
}

std::deque<std::string> Loader::LogFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())return {};

    std::deque<std::string> logs;
    std::string line;
    while (std::getline(file, line)){
        logs.push_back(line);
    }
    file.close();
    return logs;
}

void Loader::Unload(Type type) {
    if (type != Type::TEXTURE)return;

    TextureManager::GetInstance()->Unload();
}
