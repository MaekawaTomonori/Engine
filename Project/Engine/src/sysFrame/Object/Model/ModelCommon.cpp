#include "ModelCommon.h"

#include <memory>

#include "DirectX/DirectXCommon.h"
#include "DirectX/Pipeline/GraphicsPipeline.h"
#include "System/System.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

ModelCommon* ModelCommon::instance_ = nullptr;
std::once_flag ModelCommon::onceFlag_;

ModelCommon* ModelCommon::GetInstance() {
    call_once(onceFlag_, Create);
    assert(instance_);
    return instance_;
}

void ModelCommon::Create() {
    instance_ = new ModelCommon();
    SingletonFinalizer::AddFinalizer(&Finalize);
}

void ModelCommon::Finalize() {
    delete instance_;
    instance_ = nullptr;
    System::Log(Logger::Level::INFO, "ModelCommon Finalized");
}

void ModelCommon::Initialize(const std::weak_ptr<DirectXCommon>& dxCommon) {
	dxCommon_ = dxCommon;

    pipeline_ = std::make_shared<GraphicsPipeline>();
    pipeline_->Create(dxCommon_, GraphicsPipeline::Type::MODEL);
    pipeline_->SetBlendMode(BlendMode::ALPHA);

    System::Log(Logger::Level::INFO, "ModelCommon Initialized");
}

void ModelCommon::PreDraw() const {
    pipeline_->DrawCall(dxCommon_.lock()->GetCommandList());
}

std::weak_ptr<DirectXCommon> ModelCommon::GetDXCommon() const {
	return dxCommon_;
}
