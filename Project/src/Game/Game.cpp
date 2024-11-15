#include "Game.h"

#include "Framework/Framework.h"
#include "Scene/PlayScene.h"
#include "Scene/SceneFactory.h"


Game::~Game() {
}

void Game::Initialize() {
    Framework::Initialize();

    scene_->SetFactory(new SceneFactory);
    scene_->ChangeScene("Play");
}

void Game::Update() {
    Framework::Update();

}

void Game::Draw() {
    scene_->Draw();
}
