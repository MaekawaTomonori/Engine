#pragma once
#include <memory>

#include "ICommand.h"

class InputHandler {
	std::unique_ptr<ICommand> left_ = nullptr;
	std::unique_ptr<ICommand> right_ = nullptr;

public:
	void Initialize();
	ICommand* Handle() const;
};

