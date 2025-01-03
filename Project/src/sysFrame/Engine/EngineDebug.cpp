#include "EngineDebug.h"

#include "System/Loader.h"


void EngineDebug::Initialize() {
    Loader::LoadTexture("uvChecker.png");
    //Loader::LoadTexture("MonsterBall.png");
    //Loader::LoadTexture("circle.png");

    sprite_ = std::make_unique<Sprite>();
    sprite_->SetTexture("uvChecker.png");
    sprite_->Initialize();
}

void EngineDebug::Update() {
    sprite_->Update();
}

void EngineDebug::Draw() {
    sprite_->Draw();
}

void EngineDebug::Finalize() {
    sprite_.reset();
}

