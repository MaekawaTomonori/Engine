#include <Windows.h>
#include <memory>

#include "Engine/Engine.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    D3DResourceLeakChecker lc;
    std::unique_ptr<Engine> engine = std::make_unique<Engine>();
    engine->Initialize();

    //MainLoop
    while (engine->IsActive()){
        //Update
        engine->Update();
        engine->Draw();
        engine->EndFrame();
    }

    engine->Finalize();

	return 0;
}
