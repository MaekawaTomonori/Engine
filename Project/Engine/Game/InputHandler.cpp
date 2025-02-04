#include "InputHandler.h"

#include "System/Input/Input.h"

void InputHandler::Initialize() {
	left_ = std::make_unique<MoveLeftCommand>();
	right_ = std::make_unique<MoveRightCommand>();
}

ICommand* InputHandler::Handle() const {
	if (Input::GetInstance()->PushKey(DIK_D))return right_.get();
	if (Input::GetInstance()->PushKey(DIK_A))return left_.get();

	return nullptr;
}
