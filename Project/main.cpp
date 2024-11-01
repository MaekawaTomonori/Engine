
#include <memory>

#include "WindowsApplication/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"
#include "DirectX/ObjectCommon/MeshCommon.h"
#include "DirectX/Texture/TextureManager.h"
#include "DirectX/Util/D3DResourceLeakChecker.h"
#include "DirectX/ObjectCommon/SpriteCommon.h"
#include "DirectX/ObjectCommon/ModelCommon.h"
#include "System/ImGui/ImGuiManager.h"

#include "Object/Triangle.h"
#include "Object/Model/Model.h"
#include "Object/Model/Mesh/Mesh.h"
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

    winApp->Initialize("Engine");

    dxCommon->Initialize(winApp.get());

    srvManager->Initialize(dxCommon.get());

    imguiManager->Initialize();

    spriteCommon->Initialize();
    modelCommon->Initialize();

    textureManager->Initialize(dxCommon.get(), srvManager.get());
    modelManager->Initialize(dxCommon.get());

    //UserInit

    textureManager->Load("uvChecker.png");
    textureManager->Load("monsterBall.png");

    modelManager->Load("plane.obj");
    modelManager->Load("axis.obj");

    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->Initialize();


    std::shared_ptr<Model> model = std::make_shared<Model>(modelCommon.get());
    model->Initialize();
    model->SetMesh("plane.obj");
    model->SetCamera(camera.get());

    std::shared_ptr<Model> model2 = std::make_shared<Model>(modelCommon.get());
    model2->Initialize();
    model2->SetMesh("axis.obj");
    model2->SetCamera(camera.get());

    //MainLoop
    while (winApp->ProcessMessage()){
        //Update
        imguiManager->Begin();
        ImGui::ShowDemoWindow();

        camera->Update();
        model->Update();
        model2->Update();

        imguiManager->End();

        //Draw
        srvManager->PreDraw();
        dxCommon->PreDraw();

        modelCommon->PreDraw();
        model->Draw();
        model2->Draw();

        spriteCommon->PreDraw();

        imguiManager->Draw();
        dxCommon->PostDraw();
    }

	return 0;
}
