#pragma once
#include "System/Math/TransformationMatrix.h"

class Particle{
    ParticleForGPU* transformation_ = nullptr;
public:
	void Initialize(ParticleForGPU* transformationMatrix);
	void Update();
	bool IsDead() const;
};

