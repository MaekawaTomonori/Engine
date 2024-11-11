
#include <memory>

#include "WindowsApplication/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"
#include "DirectX/Lighting/LightManager.h"
#include "DirectX/Texture/TextureManager.h"
#include "DirectX/Util/D3DResourceLeakChecker.h"
#include "DirectX/ObjectCommon/SpriteCommon.h"
#include "DirectX/ObjectCommon/ModelCommon.h"
#include "System/ImGui/ImGuiManager.h"

#include "Object/Model/Model.h"
#include "Object/Sprite/Sprite.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    //EngineInit
    std::shared_ptr<D3DResourceLeakChecker> leakChecker;

    std::shared_ptr<WinApp> winApp = std::make_shared<WinApp>();
    std::shared_ptr<DirectXCommon> dxCommon = std::make_shared<DirectXCommon>();
    std::shared_ptr<SRVManager> srvManager = std::make_shared<SRVManager>();
    std::shared_ptr<ImGuiManager> imguiManager = std::make_shared<ImGuiManager>(winApp.get(), dxCommon.get(), srvManager.get());
    std::shared_ptr<SpriteCommon> spriteCommon = std::make_shared<SpriteCommon>(dxCommon.get());
    std::shared_ptr<ModelCommon> modelCommon = std::make_shared<ModelCommon>(dxCommon.get());
    std::shared_ptr<TextureManager> textureManager = TextureManager::GetInstance();
    std::shared_ptr<ModelManager> modelManager = ModelManager::GetInstance();
    std::shared_ptr<LightManager> light = LightManager::GetInstance();

    winApp->Initialize("Engine");

    dxCommon->Initialize(winApp.get());

    srvManager->Initialize(dxCommon.get());

    imguiManager->Initialize();

    spriteCommon->Initialize();
    modelCommon->Initialize();

    textureManager->Initialize(dxCommon.get(), srvManager.get());
    modelManager->Initialize(dxCommon.get());
    light->Initialize(dxCommon.get());

    //UserInit

    textureManager->Load("uvChecker.png");
    textureManager->Load("monsterBall.png");

    modelManager->Load("plane.obj");
    modelManager->Load("axis.obj");

    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->Initialize();

    std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(dxCommon.get(), spriteCommon.get());
    sprite->Initialize("uvChecker.png");
    sprite->SetCamera(camera.get());

    std::shared_ptr<Model> model = std::make_shared<Model>(modelCommon.get());
    model->Initialize();
    model->SetCamera(camera.get());
    model->SetMesh("plane.obj");

    //MainLoop
    while (winApp->ProcessMessage()){
        //Update
        imguiManager->Begin();
        ImGui::ShowDemoWindow();

        light->Update();
        camera->Update();
        sprite->Update();
        model->Update();

        imguiManager->End();

        //Draw
        srvManager->PreDraw();
        dxCommon->PreDraw();

        modelCommon->PreDraw();
        model->Draw();

        spriteCommon->PreDraw();
        sprite->Draw();

        imguiManager->Draw();
        dxCommon->PostDraw();
    }

	return 0;
}
