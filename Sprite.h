#pragma once
#include "Object/Object.h"
class SpriteBase;

class Sprite : public Object{
    SpriteBase* spriteBase_ = nullptr;

public:
	Sprite(DirectXCommon* dxCommon, SpriteBase* spriteBase) :Object(dxCommon), spriteBase_(spriteBase) {}

    void Initialize() override;
    void Update() override;
    void Draw() override;
};

