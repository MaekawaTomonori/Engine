#include "GameScene.h"

void GameScene::Initialize() {
	player_ = std::make_unique<Player>();
	player_->Initialize();

	inputHandler_ = std::make_unique<InputHandler>();
	inputHandler_->Initialize();
}

void GameScene::Update() {
	ICommand* command = inputHandler_->Handle();
	if (command != nullptr) {
		command->Execute(*player_.get());
	}

	player_->Update();
}

void GameScene::Draw() {
	player_->Draw();
}

void GameScene::Finalize() {
}
