#include "SpriteCommon.h"

#include <memory>

#include "DirectX/DirectXCommon.h"
#include "DirectX/Pipeline/GraphicsPipeline.h"
#include "DirectX/Shader/Shader.h"

void SpriteCommon::CreatePipeline() {
    pipeline_ = std::make_shared<GraphicsPipeline>();
    pipeline_->Create(dxCommon_->GetDevice(), dxCommon_->GetShader()->GetVertexShader(), dxCommon_->GetShader()->GetPixelShader());
}

void SpriteCommon::Initialize() {
    // Do something
    CreatePipeline();
}

void SpriteCommon::PreDraw() const {
    pipeline_->DrawCall(dxCommon_->GetCommandList());
    dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
