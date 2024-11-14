#pragma once
#include "Framework/Framework.h"
#include "Object/Model/Model.h"
#include "Object/Sprite/Sprite.h"

class Game : public Framework{
	std::shared_ptr<TextureManager> textureManager;
	std::shared_ptr<ModelManager> modelManager;

    std::unique_ptr<Sprite> sprite;
    std::unique_ptr<Model> model;
	//SoundData* sData {};
	bool audioTest = true;
    int count = 50;
    uint32_t audioHandle = 0;
public:
	~Game() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
};

