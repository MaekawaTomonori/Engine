#include "SpriteBase.h"

#include <memory>

#include "DirectX/DirectXCommon.h"
#include "DirectX/Pipeline/GraphicsPipeline.h"
#include "DirectX/Shader/Shader.h"

void SpriteBase::CreatePipeline() {
    pipeline_ = std::make_shared<GraphicsPipeline>();
    pipeline_->Create(dxCommon_->GetDevice(), dxCommon_->GetShader()->GetVertexShader(), dxCommon_->GetShader()->GetPixelShader());
}

void SpriteBase::Initialize() {
    // Do something
    CreatePipeline();
}

void SpriteBase::PreDraw() const {
    pipeline_->DrawCall(dxCommon_->GetCommandList());
}
