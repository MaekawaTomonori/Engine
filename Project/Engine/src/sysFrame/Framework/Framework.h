#pragma once
#include "Adapter.h"
#include "Scene/SceneManager.h"

class Framework{
protected:
	std::unique_ptr<Adapter> engine_;
	std::unique_ptr<SceneManager> scene_;

protected:
	virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Finalize() = 0;
public:
	virtual ~Framework() = default;

	void Run();
};

