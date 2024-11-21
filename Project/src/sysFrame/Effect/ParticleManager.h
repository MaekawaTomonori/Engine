#pragma once
#include <list>
#include <memory>


class ParticleCommon;
class SRVManager;
class DirectXCommon;

enum class ParticleType{
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

    static std::shared_ptr<ParticleManager> instance_;

public:
	static std::shared_ptr<ParticleManager> GetInstance();

	void Initialize(DirectXCommon* dxCommon, SRVManager* srvManager);
    void PreDraw() const;
	Emitter* Emit(/*const Vector3& position/*, ParticleType type*/);
};

