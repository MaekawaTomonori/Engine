#pragma once
#include "SpriteCommon.h"
#include "Object/Object.h"
#include "System/Math/Vector2.h"
#include "System/Math/VertexData.h"

class SpriteCommon;

class Sprite : public Object{
    SpriteCommon* spriteCommon_ = nullptr;

    VertexData* vertexData_ = nullptr;

    Vector2 position {0,0};
    Vector2 size {1,1};
    float rotation = 0.f;

    Vector2 anchorPoint {};
    bool flipX = false;
    bool flipY = false;

    Vector2 leftTop {};
    Vector2 texSize {100, 100};

    BlendMode blendMode_;

private:
	void AdjustTextureSize();

public:
	Sprite();

    void Initialize() override;
    void Initialize(const std::string& texture) override;
    void Update() override;
    void Draw() override;

    const Vector2& GetPosition() const;

    void SetPosition(const Vector2& p);

    const Vector2& GetSize() const;

    void SetSize(const Vector2& s);

    float GetRotation() const;

    void SetRotation(float r);

    const Vector4& GetColor() const;

    void SetColor(const Vector4& color) const;

    const Vector2& GetAnchorPoint() const;

    void SetAnchorPoint(const Vector2& a);

    bool IsFlipX() const;

    void SetFlipX(bool f);

    bool IsFlipY() const;

    void SetFlipY(bool f);

    const Vector2& GetTextureLeftTop() const;

    void SetTextureLeftTop(const Vector2& textureLeftTop);

    const Vector2& GetTextureSize() const;

    void SetTextureSize(const Vector2& textureSize);

    void SetBlendMode(const BlendMode mode);
};

