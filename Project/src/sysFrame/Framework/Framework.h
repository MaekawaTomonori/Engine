#pragma once
#include <memory>

#include "Engine.h"
#include "Game/Scene/SceneManager.h"

class Framework{
protected:
    //maybe replace to adapter
    std::unique_ptr<Engine> engine_;
    std::unique_ptr<SceneManager> scene_;

    bool endRequest_ = false;

protected:
	virtual void Initialize();
    virtual void Update();
    virtual void Draw() = 0;
    virtual bool IsEndRequest() const;

public:
	virtual ~Framework() = default;

    void Run();
};

