#include "Engine.h"

#include "Effect/ParticleManager.h"
#include "System/System.h"

std::unique_ptr<Camera> Engine::defaultCamera_ = std::make_unique<Camera>();

Engine::Engine() {
    log_ = Log::GetLogger();
	log_->Initialize();

    System::Log(Log::Level::INFO, "[Engine] Starting...");

    winApp_ = std::make_unique<WinApp>();
    dxCommon_ = std::make_unique<DirectXCommon>();
    imguiManager_ = std::make_unique<ImGuiManager>(winApp_.get(), dxCommon_.get(), srvManager_.get());
    srvManager_ = std::make_unique<SRVManager>();
    spriteCommon_ = SpriteCommon::GetInstance();
    modelCommon_ = ModelCommon::GetInstance();
    textureManager_ = TextureManager::GetInstance();
    modelManager_ = ModelManager::GetInstance();
    lightManager_ = LightManager::GetInstance();
    input_ = std::make_unique<Input>();
    audio_ = Audio::GetInstance();
    particle_ = ParticleManager::GetInstance();
}

void Engine::Initialize() const {
    try{
        System::Log(Log::Level::INFO, "[Engine] Initialize...");


        //EngineInit
        winApp_->Initialize("Engine");

        dxCommon_->Initialize(winApp_.get());

        srvManager_->Initialize(dxCommon_.get());

        imguiManager_->Initialize(srvManager_.get());

        spriteCommon_->Initialize(dxCommon_.get());
        modelCommon_->Initialize(dxCommon_.get());

        textureManager_->Initialize(dxCommon_.get(), srvManager_.get());
        modelManager_->Initialize(dxCommon_.get());
        lightManager_->Initialize(dxCommon_.get());
        particle_->Initialize(dxCommon_.get(), srvManager_.get());

        input_->Initialize(winApp_.get());
        audio_->Initialize();


        defaultCamera_->Initialize();

        System::Log(Log::Level::INFO, "[Engine] Enabled!");
    } catch (const std::exception& e){
        System::Log(Log::Level::ERR, std::format("Engine Initialization Failed: {}", e.what()));
        assert(0);
    }
}

void Engine::Finalize() const {
    System::Log(Log::Level::INFO, "[Engine] Finalizing...");

    audio_->Finalize();
    input_->Finalize();
    particle_->Finalize();
    lightManager_->Finalize();
    modelManager_->Finalize();
    textureManager_->Finalize();

    modelCommon_->Finalize();
    spriteCommon_->Finalize();
    imguiManager_->Finalize();

    srvManager_->Finalize();
    dxCommon_->Finalize();
    winApp_->Finalize();

    System::Log(Log::Level::INFO, "[Engine] Disabled!");
}

void Engine::Update() const {
    //Update
    input_->Update();
    imguiManager_->Begin();

#ifdef _DEBUG
    ImGui::ShowDemoWindow();
    ImGui::Begin("Engine");
    if(ImGui::Button("Log")) {
        System::Log(Log::Level::INFO, "Log Button Pressed");
    }
    ImGui::End();
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
