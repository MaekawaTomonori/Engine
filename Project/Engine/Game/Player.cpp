#include "Player.h"

void Player::Initialize() {
	sprite_ = std::make_unique<Sprite>();
	sprite_->SetTexture("player.png");
	sprite_->Initialize();
	sprite_->SetPosition({640, 360});
}

void Player::Update() {
	sprite_->Update();
}

void Player::Draw() {
	sprite_->Draw();
}

void Player::MoveRight() {
	sprite_->SetPosition({ sprite_->GetPosition().x + 1, sprite_->GetPosition().y });
}

void Player::MoveLeft() {
	sprite_->SetPosition({ sprite_->GetPosition().x - 1, sprite_->GetPosition().y });
}
