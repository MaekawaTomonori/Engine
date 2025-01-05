#pragma once
#include "Engine/Engine.h"

class Framework{
protected:
	std::unique_ptr<Engine> engine;

protected:
	virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Finalize() = 0;

public:
	virtual ~Framework() = default;

	void Run();
};

