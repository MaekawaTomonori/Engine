#pragma once
#include <memory>
#include <string>

#include "Engine/Engine.h"


class Adapter{
    std::unique_ptr<Engine> engine_;

public:
	void Initialize();
    void Update() const;
    void Draw() const;
    void EndFrame() const;
    void Finalize() const;

    bool IsActive() const;
    bool IsDebug() const;

    void SetFPSLimit(int fps) const;
    void SetTitle(const std::string& title) const;
};

