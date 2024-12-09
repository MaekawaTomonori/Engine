#include "ParticleManager.h"

#include "Emitter.h"
#include "DirectX/Heap/SRVManager.h"
#include "DirectX/ObjectCommon/ParticleCommon.h"
#include "System/System.h"

std::shared_ptr<ParticleManager> ParticleManager::instance_ = nullptr;

std::shared_ptr<ParticleManager> ParticleManager::GetInstance() {
    if (!instance_){
        instance_ = std::shared_ptr<ParticleManager>(new ParticleManager, [](const ParticleManager* ptr){
            delete ptr;
        });
    }
    return instance_;
}

void ParticleManager::Initialize(DirectXCommon* dxCommon, SRVManager* srvManager) {
    System::Log(Log::Level::INFO, "ParticleManager Enabled");
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

    System::Log(Log::Level::INFO, "ParticleManager Disabled");
}
