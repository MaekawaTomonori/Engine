#pragma once
#include "Object/Model/Model.h"
#include "Object/Particle/ParticleManager.h"
#include "Object/Sprite/Sprite.h"
#include "System/Sound/Audio.h"

class Engine;

class EngineDebug{
    Engine* engine_ = nullptr;

	ParticleKey key{};
	std::unique_ptr<Model> model_;

public:
	EngineDebug(Engine* engine);

	void Initialize();
	void Update();
	void Draw() const;
	void Finalize();
};

