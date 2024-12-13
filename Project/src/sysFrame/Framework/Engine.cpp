#include "Engine.h"

#include "Effect/ParticleManager.h"
#include "System/System.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

std::unique_ptr<Camera> Engine::defaultCamera_ = std::make_unique<Camera>();

Engine::Engine() {
    log_ = Log::GetLogger();
	log_->Initialize();

    System::Log(Log::Level::INFO, "[Engine] Starting...");

    winApp_ = std::make_unique<WinApp>();
    dxCommon_ = std::make_shared<DirectXCommon>();
    imguiManager_ = std::make_unique<ImGuiManager>(winApp_.get(), dxCommon_, srvManager_.get());
    srvManager_ = std::make_unique<SRVManager>();
    input_ = std::make_unique<Input>();

    textureManager_ = TextureManager::GetInstance();
    audio_ = Audio::GetInstance();

    spriteCommon_ = SpriteCommon::GetInstance();
    modelCommon_ = ModelCommon::GetInstance();
    modelManager_ = ModelManager::GetInstance();
    particle_ = ParticleManager::GetInstance();

    lightManager_ = LightManager::GetInstance();
}

void Engine::Initialize() const {
    try{
        System::Log(Log::Level::INFO, "[Engine] Initialize...");


        //EngineInit
        winApp_->Initialize("Engine");

        dxCommon_->Initialize(winApp_.get());

        srvManager_->Initialize(dxCommon_);

        imguiManager_->Initialize(srvManager_.get());

        spriteCommon_->Initialize(dxCommon_);
        modelCommon_->Initialize(dxCommon_);

        textureManager_->Initialize(dxCommon_, srvManager_.get());
        modelManager_->Initialize(dxCommon_);
        lightManager_->Initialize(dxCommon_);
        particle_->Initialize(dxCommon_, srvManager_.get());

        input_->Initialize(winApp_.get());
        audio_->Initialize();


        defaultCamera_->Initialize();

        System::Log(std::format("DirectXCommon : {}", dxCommon_.use_count()));

        System::Log(Log::Level::INFO, "[Engine] Enabled!");
    } catch (const std::exception& e){
        System::Log(Log::Level::ERR, std::format("Engine Initialization Failed: {}", e.what()));
        assert(0);
    }
}

void Engine::Finalize() const {
    System::Log(Log::Level::INFO, "[Engine] Finalizing...");

    SingletonFinalizer::Finalize();

    imguiManager_->Finalize();
    srvManager_->Finalize();

    OutputDebugStringA(std::format("DirectXCommon : {}", dxCommon_.use_count()).c_str());

    dxCommon_->Finalize();
    winApp_->Finalize();

    //System::Log(Log::Level::INFO, "[Engine] Disabled!");
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
