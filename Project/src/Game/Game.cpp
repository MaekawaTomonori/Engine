#include "Game.h"

#include "Object/Model/Model.h"
#include "Object/Sprite/Sprite.h"


Game::~Game() {
}

void Game::Initialize() {
    Framework::Initialize();

	//UserInit
    textureManager = TextureManager::GetInstance();
    textureManager->Load("uvChecker.png");
    textureManager->Load("monsterBall.png");

    modelManager = ModelManager::GetInstance();
    modelManager->Load("plane.obj");
    modelManager->Load("axis.obj");

	sprite = std::make_unique<Sprite>(engine_->GetDirectXCommon(), engine_->GetSpriteCommon());
    sprite->Initialize("uvChecker.png");
    sprite->SetCamera(engine_->GetDefaultCamera());

    model = std::make_unique<Model>(engine_->GetModelCommon());
    model->Initialize();
    model->SetCamera(engine_->GetDefaultCamera());
    model->SetMesh("plane.obj");

	//*sData = Audio::LoadWave("assets/Audio/Alarm01.wav");
}

void Game::Update() {
    Framework::Update();

	sprite->Update();
    model->Update();

    if (audioTest){
        //audioHandle = Audio::Play(*sData);
        audioTest = false;
    }

    if(count-- <= 0){
        //Audio::Stop(audioHandle);
    }
}

void Game::Draw() {
    engine_->ModelPreDraw();
	model->Draw();

    engine_->SpritePreDraw();
	sprite->Draw();

    engine_->PostDraw();
}
