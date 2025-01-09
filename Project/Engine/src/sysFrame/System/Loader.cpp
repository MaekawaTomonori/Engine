#include "Loader.h"

#include "DirectX/Model/ModelManager.h"
#include "DirectX/Texture/TextureManager.h"
#include "Sound/Audio.h"
#include "Thread/ThreadManager.h"

void Loader::Texture(const std::string& path) {
    //ThreadManager::GetInstance()->AddTask([&](){
	TextureManager::GetInstance()->Load(path);
    //});
}

void Loader::Model(const std::string& path) {
    ModelManager::GetInstance()->Load(path);
}

void Loader::Audio(const std::string& path) {
    Audio::GetInstance()->Load(path);
}

void Loader::Unload(Type type) {
    if (type != Type::TEXTURE)return;

    ThreadManager::GetInstance()->AddTask([](){
        TextureManager::GetInstance()->Unload();
    });
}
