#include "Emitter2d.h"

#include <algorithm>

#include "ParticleCommon.h"
#include "DirectX/DirectXCommon.h"
#include "Windows/WinApp.h"
#include "Particle2d.h"
#include "DirectX/Texture/TextureManager.h"

#include "System/Math/MathUtils.h"

void Emitter2d::Initialize() {
    commandList_ = pCommon_->GetDXCommon()->GetCommandList();

    vertexResource_.Attach(DirectXCommon::CreateBufferResource(pCommon_->GetDXCommon()->GetDevice(), sizeof(VertexData) * MAX_PARTICLE).Get());

    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
    vertexBufferView_.StrideInBytes = sizeof(VertexData);

    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

    vertexData_[0].position = {0, 1.f, 0, 1};
    vertexData_[1].position = {0, 0, 0 , 1};
    vertexData_[2].position = {1.f, 1.f, 0, 1};
    vertexData_[3].position = {1.f, 0, 0, 1};

    vertexData_[0].texcoord = {0, 1};
    vertexData_[1].texcoord = {0, 0};
    vertexData_[2].texcoord = {1, 1};
    vertexData_[3].texcoord = {1, 0};

    indexResource_.Attach(DirectXCommon::CreateBufferResource(pCommon_->GetDXCommon()->GetDevice(), sizeof(uint32_t) * 6).Get());

    indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
    indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
    indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

    indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

    indexData_[0] = 0;
    indexData_[1] = 1;
    indexData_[2] = 2;
    indexData_[3] = 1;
    indexData_[4] = 3;
    indexData_[5] = 2;

    dataResource_.Attach(DirectXCommon::CreateBufferResource(pCommon_->GetDXCommon()->GetDevice(), sizeof(ForGpu) * MAX_PARTICLE).Get());
    dataResource_->Map(0, nullptr, reinterpret_cast<void**>(&data_));

    index_ = srvManager_->Allocate();
    gpuHandle_ = srvManager_->GetGPUHandle(index_);
    srvManager_->CreateSRVforStructuredBuffer(index_, dataResource_.Get(), MAX_PARTICLE, sizeof(ForGpu));

    particles_.resize(MAX_PARTICLE);
    for (int i = 0; i < MAX_PARTICLE; ++i){
        particles_[i] = std::make_unique<Particle2d>();
        particles_[i]->Initialize();
    }
}

void Emitter2d::Update() const {
    for (const auto& p : particles_){
        if (p->IsDead())continue;

        p->Update();
    }
}

void Emitter2d::Draw() {
    activeCount_ = 0;
    for (const auto& p : particles_){
        if(p->IsDead()) continue;

        Transform t = p->GetTransform();

        Matrix4x4 world = MathUtils::Matrix::MakeAffineMatrix(t.scale, t.rotate, t.translate - (t.scale / 2.f));
        Matrix4x4 vp = MathUtils::Matrix::MakeIdentity() * MathUtils::Matrix::MakeOrthogonalMatrix(0, WinApp::CLIENT_WIDTH, 0, WinApp::CLIENT_HEIGHT, 0, 100.f);
    	data_[activeCount_].wvp = world * vp;
        data_[activeCount_].color = p->GetColor();
        ++activeCount_;
    }
    pCommon_->Draw();
    
    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
    commandList_->IASetIndexBuffer(&indexBufferView_);
    commandList_->SetGraphicsRootDescriptorTable(0, gpuHandle_);
    commandList_->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetGPUHandle(texture_));
    commandList_->DrawIndexedInstanced(6, activeCount_, 0, 0, 0);
}

#undef min
#undef max

void Emitter2d::Emit() {
    uint32_t emitCountLeft = config_.count;
    for (auto& p : particles_){
        if (p->IsDead()){
        	p->Enable(config_.position + Vector2{MathUtils::Random(config_.min.x, config_.max.x), MathUtils::Random(config_.min.y, config_.max.y)});
			p->SetLifetime(config_.lifetime);
            Vector2 direction = config_.direction;
            if (config_.randomizeDirection){
                direction = {MathUtils::Random(-1, 1), MathUtils::Random(-1, 1)};
            }
            direction.Normalize();
            config_.speed.x = std::min(config_.speed.x, 0.f);
            p->SetVelocity({  direction.x * MathUtils::Random(config_.speed.x, config_.speed.y), direction.y * MathUtils::Random(config_.speed.x, config_.speed.y) });
            p->SetAcceleration(config_.gravity);

            Vector4 color = config_.color;
            if (config_.randomizeColor){
                color = {MathUtils::Random(0,1), MathUtils::Random(0,1), MathUtils::Random(0,1), 1};
            }
	        p->SetColor(color);

            if (--emitCountLeft == 0)break;
        }
    }
}
