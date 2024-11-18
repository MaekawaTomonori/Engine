#include "ParticleManager.h"

#include "Emitter.h"
#include "DirectX/Heap/SRVManager.h"

void ParticleManager::Initialize(DirectXCommon* dxCommon, SRVManager* srvManager) {
    dxCommon_ = dxCommon;
    srvManager_ = srvManager;
}

Emitter* ParticleManager::Emit(const Vector3& position) {
    std::unique_ptr<Emitter> emitter = std::make_unique<Emitter>();
    emitter->Initialize(dxCommon_, srvManager_);

    emitters_.push_back(emitter);

    return emitter.get();
}
