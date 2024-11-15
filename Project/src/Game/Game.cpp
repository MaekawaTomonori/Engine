#include "Game.h"

#include "Framework/Framework.h"
#include "Scene/PlayScene.h"


Game::~Game() {
}

void Game::Initialize() {
    Framework::Initialize();

    scene_->SetNextScene(new PlayScene());
}

void Game::Update() {
    Framework::Update();

}

void Game::Draw() {
    scene_->Draw();
}
