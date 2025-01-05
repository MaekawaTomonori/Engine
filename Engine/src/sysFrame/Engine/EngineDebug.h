#pragma once
#include "Object/Model/Model.h"
#include "Object/Sprite/Sprite.h"

class EngineDebug{
    std::unique_ptr<Sprite> sprite_;
	std::unique_ptr<Model> model_;
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
};

