#include <memory>

#include "Game/Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    static std::shared_ptr<D3DResourceLeakChecker> lc =std::make_shared<D3DResourceLeakChecker>();
    Framework* framework = new Game();

    framework->Run();

    delete framework;

	return 0;
}
