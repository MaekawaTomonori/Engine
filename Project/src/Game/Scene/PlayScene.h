#pragma once
#include <memory>

#include "Scene/BaseScene.h"
#include "DirectX/Model/ModelManager.h"
#include "DirectX/Texture/TextureManager.h"
#include "Object/Model/Model.h"
#include "Object/Sprite/Sprite.h"

class Emitter;

class PlayScene : public BaseScene{
	TextureManager* textureManager;
	ModelManager* modelManager;

    std::unique_ptr<Sprite> sprite;
    std::unique_ptr<Model> model;
    Emitter* emitter_ = nullptr;
public:
	~PlayScene() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
};

