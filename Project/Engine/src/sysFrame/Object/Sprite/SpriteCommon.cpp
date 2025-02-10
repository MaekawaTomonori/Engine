#include "SpriteCommon.h"

#include <memory>

#include "DirectX/DirectXCommon.h"
#include "DirectX/Pipeline/GraphicsPipeline.h"
#include "System/System.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

SpriteCommon* SpriteCommon::instance_ = nullptr;
std::once_flag SpriteCommon::onceFlag_;

void SpriteCommon::CreatePipeline() {
    for (uint16_t i = 0; i < static_cast<uint16_t>(BlendMode::NONE); ++i){
        std::unique_ptr<GraphicsPipeline> pipeline = std::make_unique<GraphicsPipeline>();
        pipeline->SetBlendMode(static_cast<BlendMode>(i));
        pipeline->Create(dxCommon_.lock(), GraphicsPipeline::Type::SPRITE);
        pipelines_.push_back(std::move(pipeline));
    }
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

void SpriteCommon::Initialize(const std::weak_ptr<DirectXCommon>& dxCommon) {
    dxCommon_ = dxCommon;
    mode_ = BlendMode::NONE;

    // Do something
    CreatePipeline();

    System::Log(Log::Level::INFO, "SpriteCommon Enabled");
}

void SpriteCommon::PreDraw() const {
    auto dxc = dxCommon_.lock();
    if (!dxc){
        System::Log(Log::Level::ERR, "SRVManager Initialize Failed");
        return;
    }

    pipelines_[static_cast<uint16_t>(mode_)]->DrawCall(dxc->GetCommandList());
    dxc->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void SpriteCommon::SetBlendMode(const BlendMode mode) {
    mode_ = mode;
}
