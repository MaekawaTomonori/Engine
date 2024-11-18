#include "Emitter.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"
#include "DirectX/Model/ModelManager.h"
#include "Object/Model/Mesh/Mesh.h"
#include "System/Math/MathUtils.h"

void Emitter::Initialize(const DirectXCommon* dxCommon, SRVManager* srv) {
    commandList_ = dxCommon->GetCommandList();

	transformation_.Attach(DirectXCommon::CreateBufferResource(dxCommon->GetDevice(), sizeof(Particle) * MAX_COUNT));
    transformation_->Map(0, nullptr, reinterpret_cast<void**>(&particle_));

    for (uint16_t i = 0; i < MAX_COUNT; ++i){
        particle_[i] = Spawn();
    }

    srvIndex_ = srv->Allocate();
    gpuHandle_ = srv->GetGPUHandle(srvIndex_);
    srv->CreateSRVforStructuredBuffer(srvIndex_, transformation_.Get(), MAX_COUNT, sizeof(Particle));

    mesh_ = ModelManager::GetInstance()->Find("plane.obj");
}

void Emitter::Update() {
    for(uint16_t i = 0; i ++ < MAX_COUNT; ){
        
    }
}

void Emitter::Draw() const {
    commandList_->SetGraphicsRootDescriptorTable(1, gpuHandle_);
    mesh_->Draw();
}

Particle Emitter::Spawn() {
    Particle particle;
    particle.transform.translate = {0, 0, 0};
    particle.transform.rotate = {};
    particle.transform.scale = {1, 1, 1};
    particle.velocity = {};

    return particle;
}
