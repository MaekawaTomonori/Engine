#include "Game/Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    
    Framework* framework = new Game();

    framework->Run();

    delete framework;

	return 0;
}
