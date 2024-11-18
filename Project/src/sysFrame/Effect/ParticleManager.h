#pragma once
#include <list>
#include <memory>

#include "System/Math/Vector3.h"

class SRVManager;
class DirectXCommon;

enum class ParticleType{
};

class Emitter;

class ParticleManager{
	DirectXCommon* dxCommon_ = nullptr;

    SRVManager* srvManager_ = nullptr;

	std::list<std::unique_ptr<Emitter>> emitters_;

public:
	void Initialize(DirectXCommon* dxCommon, SRVManager* srvManager);
	Emitter* Emit(const Vector3& position/*, ParticleType type*/);
};

