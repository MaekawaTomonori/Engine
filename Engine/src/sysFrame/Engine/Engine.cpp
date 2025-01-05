#include "Engine.h"

#include <memory>

#include "Application/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"
#include "DirectX/Lighting/LightManager.h"
#include "DirectX/Model/ModelManager.h"
#include "DirectX/Texture/TextureManager.h"
#include "Object/Model/ModelCommon.h"
#include "Object/Sprite/SpriteCommon.h"
#include "System/System.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

std::shared_ptr<Camera> Engine::defaultCamera_ = nullptr;

void Engine::Initialize() {
    logger_ = Log::GetLogger();

	winApp_ = std::make_shared<WinApp>();
    dxCommon_ = std::make_shared<DirectXCommon>();
    srvManager_ = std::make_shared<SRVManager>();
	imguiManager_ = std::make_shared<ImGuiManager>(winApp_.get(), dxCommon_.get());

    input_ = Input::GetInstance();
    audio_ = Audio::GetInstance();

    textureManager_ = TextureManager::GetInstance();
    modelManager_ = ModelManager::GetInstance();
    spriteCommon_ = SpriteCommon::GetInstance();
    modelCommon_ = ModelCommon::GetInstance();
    light_ = LightManager::GetInstance();

	defaultCamera_ = std::make_shared<Camera>();

	winApp_->Initialize("Engine");
    dxCommon_->Initialize(winApp_.get());
    srvManager_->Initialize(dxCommon_.get());
    imguiManager_->Initialize(srvManager_.get());

    audio_->Initialize();

    input_->Initialize(winApp_.get());
    textureManager_->Initialize(dxCommon_, srvManager_.get());
    modelManager_->Initialize(dxCommon_);
    spriteCommon_->Initialize(dxCommon_);
    modelCommon_->Initialize(dxCommon_);
    light_->Initialize(dxCommon_);

    defaultCamera_->Initialize();
}

void Engine::Update() const {
    input_->Update();
    imguiManager_->Begin();
    light_->Update();
    defaultCamera_->Update();

    if (engineDebug_){
        debugScene_->Update();
    }
}

void Engine::Draw() const {
    //Draw
	imguiManager_->End();
    srvManager_->PreDraw();
    //light_->Draw();
    dxCommon_->PreDraw();

    if(engineDebug_){
        debugScene_->Draw();
    }
}

void Engine::EndFrame() const {
    imguiManager_->Draw();
    dxCommon_->PostDraw();
}

void Engine::Finalize() const {
    if (engineDebug_){
        debugScene_->Finalize();
    }
    defaultCamera_.reset();

    SingletonFinalizer::Finalize();
    srvManager_->Finalize();
    dxCommon_->Finalize();
    winApp_->Finalize();
}

bool Engine::IsActive() const {
    return winApp_->ProcessMessage();
}

void Engine::EnableDebug() {
#ifdef _DEBUG
    engineDebug_ = true;

    debugScene_ = std::make_shared<EngineDebug>();
    debugScene_->Initialize();

	System::Log(Log::Level::INFO, "DebugMode Enabled");
    return;
#endif
    //System::Log(Log::Level::ERR, "Request Cancelled!\nDebugMode is not available in Release Build.");
}

bool Engine::IsDebug() const {
	return engineDebug_;
}
