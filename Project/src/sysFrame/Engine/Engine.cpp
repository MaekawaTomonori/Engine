#include "Engine.h"

#include <memory>

#include "Application/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"
#include "DirectX/Texture/TextureManager.h"
#include "Object/Sprite/SpriteCommon.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

std::shared_ptr<Camera> Engine::defaultCamera_ = nullptr;

void Engine::Initialize() {
    logger_ = Log::GetLogger();

	winApp_ = std::make_shared<WinApp>();
    dxCommon_ = std::make_shared<DirectXCommon>();
    srvManager_ = std::make_shared<SRVManager>();
	imguiManager_ = std::make_shared<ImGuiManager>(winApp_.get(), dxCommon_.get());

    textureManager_ = TextureManager::GetInstance();
    spriteCommon_ = SpriteCommon::GetInstance();

	defaultCamera_ = std::make_shared<Camera>();

	winApp_->Initialize("Engine");
    dxCommon_->Initialize(winApp_.get());
    srvManager_->Initialize(dxCommon_.get());
    imguiManager_->Initialize();

    textureManager_->Initialize(dxCommon_, srvManager_.get());
    spriteCommon_->Initialize(dxCommon_);

    defaultCamera_->Initialize();

    if (engineDebug_){
        debugScene_ = std::make_shared<EngineDebug>();
        debugScene_->Initialize();
    }
}

void Engine::Update() const {
    imguiManager_->Begin();

    defaultCamera_->Update();

    if (engineDebug_){
        debugScene_->Update();
    }
}

void Engine::Draw() const {
	imguiManager_->End();

    //Draw
    dxCommon_->PreDraw();

    if(engineDebug_){
        debugScene_->Draw();
    }
}

void Engine::EndFrame() const {
    imguiManager_->Draw();
    dxCommon_->PostDraw();
}

void Engine::Finalize() {
    SingletonFinalizer::Finalize();
}

bool Engine::IsActive() const {
    return winApp_->ProcessMessage();
}
