#pragma once
#include "Object/Model/Model.h"
#include "Object/Particle/ParticleManager.h"
#include "Object/Sprite/Sprite.h"
#include "System/Sound/Audio.h"

class EngineDebug{
	ParticleKey key{};
	std::unique_ptr<Model> model_;

public:
	void Initialize();
	void Update();
	void Draw() const;
	void Finalize();
};

