
#include "Application/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"
#include "DirectX/Texture/TextureManager.h"
#include "DirectX/Util/D3DResourceLeakChecker.h"
#include "System/ImGui/ImGuiManager.h"
#include "Object/Sprite/SpriteBase.h"

#include "Object/Triangle.h"
#include "Object/Model/ModelBase.h"
#include "Object/Sprite/Sprite.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    //EngineInit
    std::shared_ptr<D3DResourceLeakChecker> leakChecker;

    std::shared_ptr<WinApp> winApp = std::make_shared<WinApp>();
    std::shared_ptr<DirectXCommon> dxCommon = std::make_shared<DirectXCommon>();
    std::shared_ptr<SRVManager> srvManager = std::make_shared<SRVManager>();
    std::shared_ptr<ImGuiManager> imguiManager = std::make_shared<ImGuiManager>(winApp.get(), dxCommon.get(), srvManager.get());
    std::shared_ptr<TextureManager> textureManager = TextureManager::GetInstance();
    std::shared_ptr<SpriteCommon> spriteBase = std::make_shared<SpriteCommon>(dxCommon.get());
    std::shared_ptr<ModelBase> modelBase = std::make_shared<ModelBase>(dxCommon.get());


    winApp->Initialize("Engine");

    dxCommon->Initialize(winApp.get());

    srvManager->Initialize(dxCommon.get());

    imguiManager->Initialize();

    textureManager->Initialize(dxCommon.get(), srvManager.get());

    spriteBase->Initialize();
    modelBase->Initialize();

    //UserInit

    textureManager->Load("uvChecker.png");
    textureManager->Load("monsterBall.png");

    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->Initialize();


    //MainLoop
    while (winApp->ProcessMessage()){
        //Update
        imguiManager->Begin();
        ImGui::ShowDemoWindow();

        camera->Update();

        imguiManager->End();

        //Draw
        srvManager->PreDraw();
        dxCommon->PreDraw();

        modelBase->PreDraw();

        spriteBase->PreDraw();

        imguiManager->Draw();
        dxCommon->PostDraw();
    }

	return 0;
}
