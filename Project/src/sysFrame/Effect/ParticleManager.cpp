#include "ParticleManager.h"

#include "Emitter.h"
#include "DirectX/Heap/SRVManager.h"
#include "DirectX/ObjectCommon/ParticleCommon.h"
#include "System/System.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

ParticleManager* ParticleManager::instance_ = nullptr;
std::once_flag ParticleManager::onceFlag_;

ParticleManager* ParticleManager::GetInstance() {
    call_once(onceFlag_, Create);
    assert(instance_);
    return instance_;
}

void ParticleManager::Create() {
    instance_ = new ParticleManager;
    SingletonFinalizer::AddFinalizer(&Finalize);
    System::Log(Log::Level::INFO, "ParticleManager Enabled");
}

void ParticleManager::Initialize(DirectXCommon* dxCommon, SRVManager* srvManager) {
    System::Log(Log::Level::INFO, "ParticleManager Initialized");
    dxCommon_ = dxCommon;
    srvManager_ = srvManager;
    common_ = std::make_unique<ParticleCommon>(dxCommon);
    common_->Initialize();
}

Emitter* ParticleManager::Emit(const Transform& transform) {
    Emitter* emitter = new Emitter;
    emitter->Initialize(common_.get(), srvManager_, transform);

    emitters_.push_back(std::unique_ptr<Emitter>(emitter));

    return emitter;
}

void ParticleManager::Finalize() {
    delete instance_;
    instance_ = nullptr;
    System::Log(Log::Level::INFO, "ParticleManager Disabled");
}
