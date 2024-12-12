#include "Game/Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    D3DResourceLeakChecker lc;
    Framework* framework = new Game();

    framework->Run();

    delete framework;

	return 0;
}
