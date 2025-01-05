#pragma once
#include "Scene/BaseScene.h"

class GameScene final : public BaseScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
};

