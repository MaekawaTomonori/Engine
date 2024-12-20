#include "Engine.h"

#include "Application/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Texture/TextureManager.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

std::shared_ptr<Camera> Engine::defaultCamera_ = nullptr;

void Engine::Initialize() {
    winApp_ = std::make_shared<WinApp>();
    dxCommon_ = std::make_shared<DirectXCommon>();
	imguiManager_ = std::make_shared<ImGuiManager>(winApp_.get(), dxCommon_.get());

    textureManager_ = TextureManager::GetInstance();

	defaultCamera_ = std::make_shared<Camera>();

	winApp_->Initialize("Engine");
    dxCommon_->Initialize(winApp_.get());
    imguiManager_->Initialize();

    defaultCamera_->Initialize();
}

void Engine::Update() const {
    imguiManager_->Begin();

    defaultCamera_->Update();
	
}

void Engine::Draw() const {
	imguiManager_->End();

    //Draw
    dxCommon_->PreDraw();
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
