#include "ParticleCommon.h"

void ParticleCommon::Initialize() {
    pipeline_ = std::make_unique<GraphicsPipeline>();
    pipeline_->SetBlendMode(BlendMode::ALPHA);
    pipeline_->Set(dxCommon_);
}

void ParticleCommon::Draw() const {
    pipeline_->DrawCall(dxCommon_->GetCommandList());
}
