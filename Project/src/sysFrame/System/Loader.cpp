#include "Loader.h"

#include "DirectX/Texture/TextureManager.h"

void Loader::LoadTexture(const std::string& path) {
    TextureManager::GetInstance()->Load(path);
}
