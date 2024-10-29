#pragma once
#include "Object/Object.h"
class SpriteBase;

class Sprite : public Object{
    SpriteBase* spriteBase_ = nullptr;

    Vector2 position {0,0};
    Vector2 size {1,1};
    float rotation = 0.f;
public:
	Sprite(DirectXCommon* dxCommon, SpriteBase* spriteBase) :Object(dxCommon), spriteBase_(spriteBase) {}

    void Initialize() override;
    void Update() override;
    void Draw() override;

    const Vector2& GetPosition() const {
        return position;
    }
    void SetPosition(const Vector2& p) {
        position = p;
    }

	float GetRotation() const {
        return rotation;
    }
    void SetRotation(float r) {
        rotation = r;
    }

    const Vector4& GetColor() const {
        return material_->color;
    }
    void SetColor(const Vector4& color) const {
        material_->color = color;
    }
};

