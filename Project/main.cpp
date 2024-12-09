#include <spdlog/spdlog-inl.h>

#include "Game/Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    std::shared_ptr<D3DResourceLeakChecker> lc_ = std::make_shared<D3DResourceLeakChecker>();

    Framework* framework = new Game();

    framework->Run();

    delete framework;

    spdlog::shutdown();

	return 0;
}
