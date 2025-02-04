#pragma once
#include "Object/Sprite/Sprite.h"

class Player {
	std::unique_ptr<Sprite> sprite_;
public:
	void Initialize();
	void Update();
	void Draw();

	void MoveRight();
	void MoveLeft();
};

