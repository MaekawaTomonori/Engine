#include "SpriteCommon.h"

#include <memory>

#include "DirectX/DirectXCommon.h"
#include "DirectX/Pipeline/GraphicsPipeline.h"
#include "System/System.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"
#include "System/Thread/ThreadManager.h"

SpriteCommon* SpriteCommon::instance_ = nullptr;
std::once_flag SpriteCommon::onceFlag_;

void SpriteCommon::CreatePipeline() {
    pipeline_ = std::make_shared<GraphicsPipeline>();
    pipeline_->Create(dxCommon_.lock(), GraphicsPipeline::Type::SPRITE);
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
    System::Log(Logger::Level::INFO, "SpriteCommon Disabled");
}

void SpriteCommon::Initialize(const std::weak_ptr<DirectXCommon>& dxCommon) {
    dxCommon_ = dxCommon;

    // Do something
    ThreadManager::GetInstance()->AddTask([&]{CreatePipeline();});

    System::Log(Logger::Level::INFO, "SpriteCommon Enabled");
}

void SpriteCommon::PreDraw() const {
    auto dxc = dxCommon_.lock();
    if (!dxc){
        System::Log(Logger::Level::ERR, "SRVManager Initialize Failed");
        return;
    }

    pipeline_->DrawCall(dxc->GetCommandList());
    dxc->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
