#pragma once
#include <memory>
#include "Scene/BaseScene.h"

#include "Player.h"
#include "InputHandler.h"

class GameScene final : public BaseScene {
	std::unique_ptr<InputHandler> inputHandler_;
	std::unique_ptr<Player> player_;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
};

