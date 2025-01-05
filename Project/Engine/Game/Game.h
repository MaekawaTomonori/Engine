#pragma once
#include "Framework/Framework.h"
#include "System/System.h"

#include "SceneFactory.h"

class Game final : public Framework{
protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
};

inline void Game::Initialize() {
	System::Log("Game Init");
	scene_->SetFactory(std::make_unique<SceneFactory>());
	scene_->ChangeScene("Play");
}

inline void Game::Update() {
	System::Log("Game Update");
}

inline void Game::Draw() {
	System::Log("Game Draw");
}

inline void Game::Finalize() {
	System::Log("Game Finalize");
}
