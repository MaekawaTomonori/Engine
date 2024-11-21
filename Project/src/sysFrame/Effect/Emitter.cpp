#include "Emitter.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"
#include "DirectX/Model/ModelManager.h"
#include "DirectX/ObjectCommon/ParticleCommon.h"
#include "Framework/Engine.h"
#include "Object/Camera/Camera.h"
#include "Object/Model/Mesh/Mesh.h"
#include "Utility/Math/MathUtils.h"

void Emitter::Initialize(ParticleCommon* common, SRVManager* srv) {
    common_ = common;
	commandList_ = common->GetDXCommon()->GetCommandList();

	resource_.Attach(DirectXCommon::CreateBufferResource(common->GetDXCommon()->GetDevice(), sizeof(ParticleForGPU) * MAX_COUNT));
    resource_->Map(0, nullptr, reinterpret_cast<void**>(&forGpu_));

    for (uint16_t i = 0; i < MAX_COUNT; ++i){
    	std::unique_ptr<Particle> p = std::make_unique<Particle>(Spawn());
        p->transform.translate.z = 0.1f * static_cast<float>(i);
        particle_[i] = std::move(p);

        forGpu_[i].WVP = MathUtils::Matrix::MakeIdentity();
        forGpu_[i].World = MathUtils::Matrix::MakeIdentity();
        forGpu_[i].Color = particle_[i]->color;
    }

    srvIndex_ = srv->Allocate();
    gpuHandle_ = srv->GetGPUHandle(srvIndex_);
    srv->CreateSRVforStructuredBuffer(srvIndex_, resource_.Get(), MAX_COUNT, sizeof(ParticleForGPU));

    mesh_ = ModelManager::GetInstance()->Find("plane.obj");
    mesh_->SetEnableLight(false);
    mesh_->SetEnableDrawCall(false);

    camera_ = Engine::GetDefaultCamera();
}

void Emitter::Update() const {
    for(uint16_t i = 0; i < MAX_COUNT; ++i){
        particle_[i]->transform.translate += particle_[i]->velocity;
        //particle_[i]->transform.rotate.y += 0.01f;

        forGpu_[i].World = MathUtils::Matrix::MakeAffineMatrix(particle_[i]->transform);
        forGpu_[i].WVP = forGpu_[i].World * camera_->GetViewProjection();
        forGpu_[i].Color = particle_[i]->color;
    }
}

void Emitter::Draw() const {
    if (!mesh_)return;
    common_->PreDraw();

    mesh_->Draw();

	commandList_->SetGraphicsRootDescriptorTable(1, gpuHandle_);
    commandList_->DrawInstanced(static_cast<UINT>(mesh_->GetModelData().vertices.size()), MAX_COUNT, 0, 0);
}

void Emitter::SetCamera(Camera* camera) {
    camera_ = camera;
}

Particle Emitter::Spawn() {
    Particle particle;
    particle.transform.translate = {0, 0, 0};
    particle.transform.rotate = {0,0,0};
    particle.transform.scale = {1, 1, 1};
    particle.velocity = {};
    particle.color = {1,1,1,1};

    return particle;
}
