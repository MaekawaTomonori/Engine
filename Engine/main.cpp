#include "Game/Game.h"

#include "DirectX/Util/D3DResourceLeakChecker.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    //念のためのリークチェッカー
    D3DResourceLeakChecker lc;
    std::unique_ptr<Framework> fw = std::make_unique<Game>();

    fw->Run();

	return 0;
}
