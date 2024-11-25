#include "Emitter.h"

#include <numbers>
#include <algorithm>

#include "Framework/Engine.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"
#include "DirectX/Model/ModelManager.h"
#include "DirectX/ObjectCommon/ParticleCommon.h"
#include "Object/Camera/Camera.h"
#include "Object/Model/Mesh/Mesh.h"
#include "Utility/Collision.h"
#include "Utility/Math/MathUtils.h"

void Emitter::Initialize(ParticleCommon* common, SRVManager* srv, const Transform& transform) {
    common_ = common;
	commandList_ = common->GetDXCommon()->GetCommandList();

    transform_ = transform;

	resource_.Attach(DirectXCommon::CreateBufferResource(common->GetDXCommon()->GetDevice(), sizeof(ParticleForGPU) * MAX_COUNT));
    resource_->Map(0, nullptr, reinterpret_cast<void**>(&forGpu_));

    for (uint16_t i = 0; i < MAX_COUNT/10; ++i){
        particle_.splice(particle_.end(), Spawn());
    }

    srvIndex_ = srv->Allocate();
    gpuHandle_ = srv->GetGPUHandle(srvIndex_);
    srv->CreateSRVforStructuredBuffer(srvIndex_, resource_.Get(), MAX_COUNT, sizeof(ParticleForGPU));

    mesh_ = ModelManager::GetInstance()->Find("plane");
    mesh_->SetEnableLight(false);
    mesh_->SetEnableDrawCall(false);
    mesh_->SetTexture("circle.png");

    camera_ = Engine::GetDefaultCamera();

    backToFront = MathUtils::Matrix::MakeRotateY(std::numbers::pi_v<float>);

    field_.acceleration = {15.f, 0.f, 0.f};
    field_.area = {{-1.f, -1.f, 9.f}, {1.f, 1.f,11.f}};
}

void Emitter::Update() {
	constexpr float DELTA_TIME = 1.f / 60.f;
    Matrix4x4 billboard = backToFront * camera_->GetCameraMatrix();
    billboard.matrix[3][0] = 0;
    billboard.matrix[3][1] = 0;
    billboard.matrix[3][2] = 0;
    instanceCount = 0;
    for (auto itr = particle_.begin(); itr != particle_.end(); ){
        if ((*itr)->lifeTime <= (*itr)->currentTime){
            itr = particle_.erase(itr);
            continue;
        }

        (*itr)->currentTime += DELTA_TIME;
        float alpha = (1.f - (*itr)->currentTime / (*itr)->lifeTime);

        if(Collision::IsCollision(field_.area, (*itr)->transform.translate)){
            (*itr)->velocity += field_.acceleration * DELTA_TIME;
        }
        (*itr)->transform.translate += (*itr)->velocity * DELTA_TIME;

        forGpu_[instanceCount].World = MathUtils::Matrix::MakeAffineMatrix(
            MathUtils::Matrix::MakeScaleMatrix((*itr)->transform.scale),
            billboard,
            MathUtils::Matrix::MakeTranslateMatrix((*itr)->transform.translate)
        );
        forGpu_[instanceCount].WVP = forGpu_[instanceCount].World * camera_->GetViewProjection();
        forGpu_[instanceCount].Color = (*itr)->color;
        forGpu_[instanceCount].Color.w = alpha;
        ++instanceCount;
        ++itr;
    }

    if(0 < frequency_){
        frequencyTime += DELTA_TIME;

        if (frequency_<= frequencyTime){
            particle_.splice(particle_.end(), Spawn());

            frequencyTime -= frequency_;
        }
    }




    ImGui::Begin("Emitter");
    ImGui::DragFloat("Frequency", &frequency_, 0.1f, 0.f, 10.f);
    if (ImGui::Button("Spawn")){
        for (uint16_t i = 0; i < SPAWN_COUNT; ++i){
            if (particle_.size() < MAX_COUNT){
                particle_.splice(particle_.end(), Spawn());
            }
        }
    }

    ImGui::End();
}

void Emitter::Draw() const {
    if (!mesh_)return;
    common_->PreDraw();

    mesh_->Draw();

	commandList_->SetGraphicsRootDescriptorTable(1, gpuHandle_);
    commandList_->DrawInstanced(static_cast<UINT>(mesh_->GetModelData().vertices.size()), instanceCount, 0, 0);
}

void Emitter::SetCamera(Camera* camera) {
    camera_ = camera;
}

void Emitter::SetFrequency(float frequency) {
    this->frequency_ = frequency;
}

void Emitter::SetTexture(const std::string& textureName) const {
    mesh_->SetTexture(textureName);
}

std::list<std::unique_ptr<Particle>> Emitter::Spawn() const {
    std::list<std::unique_ptr<Particle>> particles;
    for (uint16_t i = 0; i < SPAWN_COUNT; ++i){
        if (MAX_COUNT <= (particle_.size() + particles.size()))break;
        std::unique_ptr<Particle> p = std::make_unique<Particle>();
        p->transform = transform_;
        p->transform.translate += Vector3::Random();
        p->transform.scale = {1, 1, 1};
        p->velocity = Vector3::Random();
        p->color = {MathUtils::Random(0, 1), MathUtils::Random(0, 1), MathUtils::Random(0, 1), 1};
        p->lifeTime = MathUtils::Random(1, 5);
        p->currentTime = 0;
        particles.push_back(std::move(p));
    }

    return particles;
}
