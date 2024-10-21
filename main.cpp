#include <cstdint>
#include <Windows.h>

#include "Application/WinApp.h"
#include "DirectX/DirectXCommon.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    std::shared_ptr<WinApp> app = std::make_shared<WinApp>();
    std::shared_ptr<DirectXCommon> dxCommon = std::make_shared<DirectXCommon>();

    app->Initialize("Engine");

    dxCommon->Initialize(app);

    //MainLoop
    while (app->ProcessMessage()){
	    
    }

	return 0;
}
