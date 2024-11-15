#pragma once
#include "Scene/BaseScene.h"
#include "System/Input/Input.h"

class TitleScene : public BaseScene{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	Input* input_ = nullptr;

};

