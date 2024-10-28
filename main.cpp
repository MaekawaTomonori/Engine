#include "Application/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"
#include "DirectX/Texture/TextureManager.h"
#include "DirectX/Util/D3DResourceLeakChecker.h"
#include "System/ImGui/ImGuiManager.h"

#include "Object/Triangle.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    //EngineInit
    std::shared_ptr<D3DResourceLeakChecker> leakChecker;

    std::shared_ptr<WinApp> winApp = std::make_shared<WinApp>();
    std::shared_ptr<DirectXCommon> dxCommon = std::make_shared<DirectXCommon>();
    std::shared_ptr<SRVManager> srvManager = std::make_shared<SRVManager>();
    std::shared_ptr<ImGuiManager> imguiManager = std::make_shared<ImGuiManager>(winApp.get(), dxCommon.get(), srvManager.get());
    std::shared_ptr<TextureManager> textureManager = TextureManager::GetInstance();

    winApp->Initialize("Engine");

    dxCommon->Initialize(winApp);

    srvManager->Initialize(dxCommon.get());

    imguiManager->Initialize();

    textureManager->Initialize(dxCommon.get(), srvManager.get());

    //UserInit

    textureManager->Load("uvChecker.png");


    std::unique_ptr<Triangle> triangle = std::make_unique<Triangle>(dxCommon.get());
    triangle->Initialize();

    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->Initialize();

    triangle->SetCamera(camera.get());

    //MainLoop
    while (winApp->ProcessMessage()){
        //Update
        imguiManager->Begin();
        ImGui::ShowDemoWindow();

        camera->Update();
        triangle->Update();

        imguiManager->End();

        //Draw
        dxCommon->PreDraw();
        srvManager->PreDraw();

        triangle->Draw();


        imguiManager->Draw();
        dxCommon->PostDraw();
    }

	return 0;
}
