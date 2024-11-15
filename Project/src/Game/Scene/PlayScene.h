#pragma once
#include <memory>

#include "Scene/BaseScene.h"
#include "DirectX/Model/ModelManager.h"
#include "DirectX/Texture/TextureManager.h"
#include "Object/Model/Model.h"
#include "Object/Sprite/Sprite.h"

class PlayScene : public BaseScene{
	std::shared_ptr<TextureManager> textureManager;
	std::shared_ptr<ModelManager> modelManager;

    std::unique_ptr<Sprite> sprite;
    std::unique_ptr<Model> model;
public:
	~PlayScene() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
};

