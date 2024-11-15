#include "ModelCommon.h"

#include <memory>

#include "DirectX/DirectXCommon.h"
#include "DirectX/Pipeline/GraphicsPipeline.h"
#include "DirectX/Shader/Shader.h"
#include "System/System.h"

std::shared_ptr<ModelCommon> ModelCommon::instance_ = nullptr;

ModelCommon::ModelCommon() {
    System::Log("ModelCommon Enabled\n");
}

ModelCommon::~ModelCommon() {
    System::Log("ModelCommon Disabled\n");
}

std::shared_ptr<ModelCommon> ModelCommon::GetInstance() {
    if (!instance_){
        instance_ = std::shared_ptr<ModelCommon>(new ModelCommon, [](const ModelCommon* ptr) {
            delete ptr;
        });
    }
    return instance_;
}

void ModelCommon::Initialize(DirectXCommon* dxCommon) {
    dxCommon_ = dxCommon;

    pipeline_ = std::make_shared<GraphicsPipeline>();
    pipeline_->Create(dxCommon_, Type::MODEL);
}

void ModelCommon::PreDraw() const {
    pipeline_->DrawCall(dxCommon_->GetCommandList());
}

DirectXCommon* ModelCommon::GetDXCommon() const {
	return dxCommon_;
}
