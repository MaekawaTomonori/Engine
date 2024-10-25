#include "Application/WinApp.h"
#include "DirectX/DirectXCommon.h"


#include "Object/Triangle.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    std::shared_ptr<WinApp> winApp = std::make_shared<WinApp>();
    std::shared_ptr<DirectXCommon> dxCommon = std::make_shared<DirectXCommon>();

    winApp->Initialize("Engine");

    dxCommon->Initialize(winApp);

    std::unique_ptr<Triangle> triangle = std::make_unique<Triangle>(dxCommon.get());
    triangle->Initialize();

    //MainLoop
    while (winApp->ProcessMessage()){
        //Update

        //Draw
        dxCommon->PreDraw();

        triangle->Draw();

        dxCommon->PostDraw();
    }

	return 0;
}
