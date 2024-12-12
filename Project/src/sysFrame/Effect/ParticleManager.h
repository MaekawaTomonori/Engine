#pragma once
#include <list>
#include <memory>
#include <mutex>

#include "Utility/Math/Transform.h"


class ParticleCommon;
class SRVManager;
class DirectXCommon;

enum class ParticleType{
	BUBBLE,
};

class Emitter;

class ParticleManager{
	DirectXCommon* dxCommon_ = nullptr;

    SRVManager* srvManager_ = nullptr;

	std::unique_ptr<ParticleCommon> common_;

	std::list<std::unique_ptr<Emitter>> emitters_;

private:
	ParticleManager() = default;
	~ParticleManager() = default;

    static ParticleManager* instance_;
	static std::once_flag onceFlag_;
public:
	static ParticleManager* GetInstance();
	static void Create();
	static void Finalize();

	void Initialize(DirectXCommon* dxCommon, SRVManager* srvManager);
	Emitter* Emit(const Transform& transform /*, ParticleType type*/);
};

