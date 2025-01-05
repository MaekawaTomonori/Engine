#pragma once
#include "Framework/Framework.h"
#include "System/System.h"

class Game final : public Framework{
protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
};

inline void Game::Initialize() {
	System::Log("Game Init");
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
