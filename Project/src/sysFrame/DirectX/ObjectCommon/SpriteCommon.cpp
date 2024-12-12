#include "SpriteCommon.h"

#include <memory>

#include "DirectX/DirectXCommon.h"
#include "DirectX/Pipeline/GraphicsPipeline.h"
#include "System/System.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

SpriteCommon* SpriteCommon::instance_ = nullptr;
std::once_flag SpriteCommon::onceFlag_;

void SpriteCommon::CreatePipeline() {
    pipeline_ = std::make_shared<GraphicsPipeline>();
    pipeline_->Create(dxCommon_, GraphicsPipeline::Type::SPRITE);
}

SpriteCommon* SpriteCommon::GetInstance() {
    call_once(onceFlag_, Create);
    assert(instance_);
    return instance_;
}

void SpriteCommon::Create() {
    instance_ = new SpriteCommon();
    SingletonFinalizer::AddFinalizer(&Destroy);
}

void SpriteCommon::Destroy() {
    delete instance_;
    instance_ = nullptr;
    System::Log(Log::Level::INFO, "SpriteCommon Disabled");
}

void SpriteCommon::Initialize(DirectXCommon* dxCommon) {
    dxCommon_ = dxCommon;

    // Do something
    CreatePipeline();

    System::Log(Log::Level::INFO, "SpriteCommon Enabled");
}

void SpriteCommon::PreDraw() const {
    pipeline_->DrawCall(dxCommon_->GetCommandList());
    dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
