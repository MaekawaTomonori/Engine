#pragma once
#include "Object/Object.h"
class SpriteCommon;

class Sprite : public Object{
    SpriteCommon* spriteBase_ = nullptr;

    Vector2 position {0,0};
    Vector2 size {1,1};
    float rotation = 0.f;

    Vector2 anchorPoint {};
    bool flipX = false;
    bool flipY = false;

    Vector2 leftTop {};
    Vector2 texSize {100, 100};

private:
	void AdjustTextureSize();

public:
	Sprite(DirectXCommon* dxCommon, SpriteCommon* spriteBase) :Object(dxCommon), spriteBase_(spriteBase) {}

    void Initialize() override;
    void Initialize(const std::string& texture);
    void Update() override;
    void Draw() override;

    const Vector2& GetPosition() const {
        return position;
    }
    void SetPosition(const Vector2& p) {
        position = p;
    }

    const Vector2& GetSize() const {
        return size;
    }
    void SetSize(const Vector2& s) {
        size = s;
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

    const Vector2& GetAnchorPoint() const {
        return anchorPoint;
    }
    void SetAnchorPoint(const Vector2& a) {
        anchorPoint = a;
    }

    bool IsFlipX() const {
        return flipX;
    }
    void SetFlipX(bool f) {
        flipX = f;
    }

    bool IsFlipY() const {
        return flipY;
    }
    void SetFlipY(bool f) {
        flipY = f;
    }

    const Vector2& GetTextureLeftTop() const {
        return leftTop;
    }
    void SetTextureLeftTop(const Vector2& textureLeftTop) {
        leftTop = textureLeftTop;
    }

    const Vector2& GetTextureSize() const {
        return texSize;
    }
    void SetTextureSize(const Vector2& textureSize) {
        texSize = textureSize;
    }
};

