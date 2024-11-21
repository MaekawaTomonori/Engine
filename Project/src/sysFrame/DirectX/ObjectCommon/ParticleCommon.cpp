#include "ParticleCommon.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/Pipeline/GraphicsPipeline.h"

void ParticleCommon::Initialize() {
    pipeline_ = std::make_unique<GraphicsPipeline>();
    pipeline_->Create(dxCommon_, GraphicsPipeline::Type::PARTICLE);
    pipeline_->SetBlendMode(BlendMode::ALPHA);
}

void ParticleCommon::PreDraw() const {
    pipeline_->DrawCall(dxCommon_->GetCommandList());
}
