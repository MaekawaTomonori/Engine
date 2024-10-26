#include "Application/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "System/ImGui/ImGuiManager.h"

#include "Object/Triangle.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    std::shared_ptr<WinApp> winApp = std::make_shared<WinApp>();
    std::shared_ptr<DirectXCommon> dxCommon = std::make_shared<DirectXCommon>();

    winApp->Initialize("Engine");

    dxCommon->Initialize(winApp);

    std::shared_ptr<ImGuiManager> imguiManager = std::make_shared<ImGuiManager>(winApp.get(), dxCommon.get());
    imguiManager->Initialize();

    std::unique_ptr<Triangle> triangle = std::make_unique<Triangle>(dxCommon.get());
    triangle->Initialize();

    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->Initialize();

    triangle->SetCamera(camera.get());

    //MainLoop
    while (winApp->ProcessMessage()){
        //Update
        imguiManager->Begin();

        camera->Update();
        triangle->Update();

        imguiManager->End();

        //Draw
        dxCommon->PreDraw();

        triangle->Draw();


        imguiManager->Draw();
        dxCommon->PostDraw();
    }

	return 0;
}
