#include "Engine.h"

#include <memory>

#include "System/System.h"

#include "Application/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"
#include "DirectX/Lighting/LightManager.h"
#include "DirectX/Model/ModelManager.h"
#include "DirectX/Texture/TextureManager.h"
#include "Object/Model/ModelCommon.h"
#include "Object/Sprite/SpriteCommon.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

void Engine::Initialize() {
    logger_ = Log::GetLogger();

	winApp_ = std::make_shared<WinApp>();
    dxCommon_ = std::make_shared<DirectXCommon>();
    srvManager_ = std::make_shared<SRVManager>();

    input_ = Input::GetInstance();
    audio_ = AudioAnther::GetInstance();

	imguiManager_ = ImGuiManager::GetInstance();
    cameraManager_ = CameraManager::GetInstance();
    textureManager_ = TextureManager::GetInstance();
    modelManager_ = ModelManager::GetInstance();
    spriteCommon_ = SpriteCommon::GetInstance();
    modelCommon_ = ModelCommon::GetInstance();
    light_ = LightManager::GetInstance();

    particleManager_ = ParticleManager::GetInstance();

	winApp_->Initialize(title_);
    dxCommon_->Initialize(winApp_.get());
    srvManager_->Initialize(dxCommon_.get());
    imguiManager_->Initialize(winApp_.get(), dxCommon_.get(), srvManager_.get());

    dxCommon_->EnablePP(srvManager_.get());

    audio_->Initialize();

    input_->Initialize(winApp_.get());
    textureManager_->Initialize(dxCommon_, srvManager_.get());
    cameraManager_->Initialize();
    particleManager_->Initialize(dxCommon_.get(), srvManager_.get());
    modelManager_->Initialize(dxCommon_);
    spriteCommon_->Initialize(dxCommon_);
    modelCommon_->Initialize(dxCommon_);
    light_->Initialize(dxCommon_);
}

void Engine::Update() const {
    input_->Update();
    imguiManager_->Begin();
    light_->Update();
    cameraManager_->Update();

    particleManager_->Update();

    if(input_->TriggerKey(DIK_RETURN)){
        winApp_->ToggleFullscreen();
    }

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
    particleManager_->Draw();
}

void Engine::EndFrame() const {
	//particleManager_->Draw();
    dxCommon_->PostDraw();
    imguiManager_->Draw();
    dxCommon_->EndFrame();
}

void Engine::Finalize() const {
    if (engineDebug_){
        debugScene_->Finalize();
    }

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

void Engine::SetBackColor(const Vector4& color) const {
    dxCommon_->SetBackColor(color);
}

void Engine::SetTitle(const std::string& title) {
    title_ = title;
    winApp_->SetTitle(title_);
}

void Engine::SetFPSLimit(const int fps) const {
	dxCommon_->SetFPSLimit(fps);
}
