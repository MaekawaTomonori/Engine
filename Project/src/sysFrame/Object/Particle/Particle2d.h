#pragma once
#include "System/Math/Transform.h"
#include "System/Math/Vector2.h"
#include "System/Math/Vector4.h"

struct Matrix4x4;

class Particle2d{
	Transform transform_ {};
    Vector2 velocity_ {};
    Vector2 acceleration_ {};

	Vector4 color_{};

	int timer  = 0;
    int lifetime_ = 0;

	const int DEFAULT_LIFETIME = 50;

public:
	~Particle2d() = default;
	void Initialize();
	void Enable(const Vector2& position);
    void Update();

    bool IsDead() const {
        return timer <= 0;
    }

    Vector4 GetColor() const {
        return color_;
    }

    Transform GetTransform() const {
        return transform_;
    }

    void SetVelocity(const Vector2& velo) {
        velocity_ = velo;
    }

    void SetAcceleration(const Vector2& acc) {
        acceleration_ = acc;
    }

    void SetColor(const Vector4& color) {
        color_ = color;
    }
	void SetLifetime(int lifetime) {
		lifetime_ = lifetime;
        timer = lifetime_;
	}
};

