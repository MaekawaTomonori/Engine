#include "Particle.h"

void Particle::Initialize(ParticleForGPU* transformationMatrix) {
    transformation_ = transformationMatrix;


}

void Particle::Update() {
}

bool Particle::IsDead() const {
}
