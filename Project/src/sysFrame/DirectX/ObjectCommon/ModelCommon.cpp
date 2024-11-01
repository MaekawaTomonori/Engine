#include "ModelCommon.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/Pipeline/GraphicsPipeline.h"
#include "DirectX/Shader/Shader.h"

void ModelCommon::Initialize() {
    pipeline_ = std::make_shared<GraphicsPipeline>();
    pipeline_->Create(dxCommon_->GetDevice(), dxCommon_->GetShader()->GetVertexShader(), dxCommon_->GetShader()->GetPixelShader());

}

void ModelCommon::PreDraw() const {
    pipeline_->DrawCall(dxCommon_->GetCommandList());
}

DirectXCommon* ModelCommon::GetDXCommon() const {
	return dxCommon_;
}
