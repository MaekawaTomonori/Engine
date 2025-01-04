#include "Loader.h"

#include "DirectX/Model/ModelManager.h"
#include "DirectX/Texture/TextureManager.h"

void Loader::Texture(const std::string& path) {
    TextureManager::GetInstance()->Load(path);
}

void Loader::Model(const std::string& path) {
    ModelManager::GetInstance()->Load(path);
}
