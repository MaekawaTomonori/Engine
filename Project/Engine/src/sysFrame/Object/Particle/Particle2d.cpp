#include "Particle2d.h"

void Particle2d::Initialize() {
    transform_ = {};
    lifetime_ = DEFAULT_LIFETIME;
}

void Particle2d::Enable(const Vector2& position) {
    transform_ = {
        {50,50,0},
        {0,0,0},
        {0,0,0}
    };
    transform_.translate = {position.x, position.y, 0};
    timer = lifetime_;
    color_ = {1, 1, 1, 1};
}

void Particle2d::Update() {
    --timer;

    // Movement
	velocity_ += acceleration_;
	transform_.translate += velocity_;

    color_.w -= 1.f / static_cast<float>(lifetime_);
}
