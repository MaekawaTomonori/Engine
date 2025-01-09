#include "EngineDebug.h"

#include "System/Loader.h"


void EngineDebug::Initialize() {
    Loader::Texture("uvChecker.png");
    Loader::Texture("MonsterBall.png");
    Loader::Texture("circle.png");

	//Loader::Model("plane");

    //Loader::Audio("Alarm01.wav");

    sprite_ = std::make_unique<Sprite>();
    sprite_->SetTexture("uvChecker.png");
    sprite_->Initialize();

    //model_ = std::make_unique<Model>();
    //model_->Initialize();
    //model_->SetMesh("plane");
}

void EngineDebug::Update() {
    sprite_->Update();
    //model_->Update();
}

void EngineDebug::Draw() const {
    sprite_->Draw();
    //model_->Draw();
}

void EngineDebug::Finalize() {
    sprite_.reset();
    //model_.reset();
}
