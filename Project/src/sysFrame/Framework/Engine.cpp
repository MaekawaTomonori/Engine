#include "Engine.h"

std::unique_ptr<Camera> Engine::defaultCamera_ = nullptr;

Engine::Engine() {
    winApp_ = std::make_unique<WinApp>();
    dxCommon_ = std::make_unique<DirectXCommon>();
    srvManager_ = std::make_unique<SRVManager>();
    imguiManager_ = std::make_unique<ImGuiManager>(winApp_.get(), dxCommon_.get(), srvManager_.get());
    spriteCommon_ = SpriteCommon::GetInstance();
    modelCommon_ = ModelCommon::GetInstance();
    textureManager_ = TextureManager::GetInstance();
    modelManager_ = ModelManager::GetInstance();
    lightManager_ = LightManager::GetInstance();
    input_ = std::make_unique<Input>();
    audio_ = std::make_unique<Audio>();

    defaultCamera_ = std::make_unique<Camera>();
}

void Engine::Initialize() const {
	//EngineInit
    winApp_->Initialize("Engine");

    dxCommon_->Initialize(winApp_.get());

    srvManager_->Initialize(dxCommon_.get());

    imguiManager_->Initialize();

    spriteCommon_->Initialize(dxCommon_.get());
    modelCommon_->Initialize(dxCommon_.get());

    textureManager_->Initialize(dxCommon_.get(), srvManager_.get());
    modelManager_->Initialize(dxCommon_.get());
    lightManager_->Initialize(dxCommon_.get());

    input_->Initialize(winApp_.get());
    audio_->Initialize();

    defaultCamera_->Initialize();
}

void Engine::Update() const {
    //Update
    input_->Update();
    imguiManager_->Begin();

#ifdef _DEBUG
    ImGui::ShowDemoWindow();
#endif

    lightManager_->Update();
    defaultCamera_->Update();
}

void Engine::PreDraw() const {
	imguiManager_->End();

    //PreDraw
    srvManager_->PreDraw();
    dxCommon_->PreDraw();
}

void Engine::PostDraw() const {
    imguiManager_->Draw();
    dxCommon_->PostDraw();
}

bool Engine::IsEndRequest() const {
    return !winApp_->ProcessMessage();
}


Camera* Engine::GetDefaultCamera() {
    if (!defaultCamera_){
        defaultCamera_ = std::make_unique<Camera>();
        defaultCamera_->Initialize();
    }
    return defaultCamera_.get();
}

DirectXCommon* Engine::GetDirectXCommon() const {
    return dxCommon_.get();
}

SpriteCommon* Engine::GetSpriteCommon() const {
    return spriteCommon_.get();
}

ModelCommon* Engine::GetModelCommon() const {
    return modelCommon_.get();
}
