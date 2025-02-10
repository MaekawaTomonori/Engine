#pragma once
#include "ISceneEffect.h"
#include "Object/Sprite/Sprite.h"

class Fade : public ISceneEffect{
    std::unique_ptr<Sprite> filterSprite_ = nullptr;
    Vector4 filterColor_ = {};
protected:
	void InitIn() override;
	bool UpdateIn() override;
	void InitOut() override;
	bool UpdateOut() override;

public:
	void Draw() override;
};

