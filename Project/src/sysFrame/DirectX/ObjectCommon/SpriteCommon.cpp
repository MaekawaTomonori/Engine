#include "SpriteCommon.h"

#include <memory>

#include "DirectX/DirectXCommon.h"
#include "DirectX/Pipeline/GraphicsPipeline.h"
#include "DirectX/Shader/Shader.h"

std::shared_ptr<SpriteCommon> SpriteCommon::instance_ = nullptr;

void SpriteCommon::CreatePipeline() {
    pipeline_ = std::make_shared<GraphicsPipeline>();
    pipeline_->Create(dxCommon_, Type::SPRITE);
}

std::shared_ptr<SpriteCommon> SpriteCommon::GetInstance() {
    if (!instance_){
        instance_ = std::shared_ptr<SpriteCommon>(new SpriteCommon, [](const SpriteCommon* ptr){
            delete ptr;
        });
    }
    return instance_;
}

void SpriteCommon::Initialize(DirectXCommon* dxCommon) {
    dxCommon_ = dxCommon;

    // Do something
    CreatePipeline();
}

void SpriteCommon::PreDraw() const {
    pipeline_->DrawCall(dxCommon_->GetCommandList());
    dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
