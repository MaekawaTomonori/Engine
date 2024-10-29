#include "Sprite.h"
#include "Application/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"
#include "DirectX/Texture/TextureManager.h"
#include "DirectX/Util/D3DResourceLeakChecker.h"
#include "System/ImGui/ImGuiManager.h"

#include "Object/Triangle.h"
#include "Object/Sprite/SpriteBase.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    //EngineInit
    std::shared_ptr<D3DResourceLeakChecker> leakChecker;

    std::shared_ptr<WinApp> winApp = std::make_shared<WinApp>();
    std::shared_ptr<DirectXCommon> dxCommon = std::make_shared<DirectXCommon>();
    std::shared_ptr<SRVManager> srvManager = std::make_shared<SRVManager>();
    std::shared_ptr<ImGuiManager> imguiManager = std::make_shared<ImGuiManager>(winApp.get(), dxCommon.get(), srvManager.get());
    std::shared_ptr<TextureManager> textureManager = TextureManager::GetInstance();
    std::shared_ptr<SpriteBase> spriteBase = std::make_shared<SpriteBase>(dxCommon.get());


    winApp->Initialize("Engine");

    dxCommon->Initialize(winApp.get());

    srvManager->Initialize(dxCommon.get());

    imguiManager->Initialize();

    textureManager->Initialize(dxCommon.get(), srvManager.get());

    spriteBase->Initialize();

    //UserInit

    textureManager->Load("uvChecker.png");

    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->Initialize();

    std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(dxCommon.get(), spriteBase.get());
    sprite->Initialize();

    //MainLoop
    while (winApp->ProcessMessage()){
        //Update
        imguiManager->Begin();
        ImGui::ShowDemoWindow();

        camera->Update();

        sprite->Update();

        imguiManager->End();

        //Draw
        srvManager->PreDraw();
        dxCommon->PreDraw();

        spriteBase->PreDraw();

        sprite->Draw();

        imguiManager->Draw();
        dxCommon->PostDraw();
    }

	return 0;
}
