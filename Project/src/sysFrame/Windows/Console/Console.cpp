#include "Console.h"

#include <Windows.h>

#include "System/System.h"

[[nodiscard]]
bool Console::Create(const std::string& title) {
    if(AllocConsole() == 0){
        return false;
    }

    FILE* fp = nullptr;
    if(freopen_s(&fp, "CONOUT$", "w", stdout)){return false;}
    if(freopen_s(&fp, "CONOUT$", "w", stderr)){return false;}
    if (freopen_s(&fp, "CONIN$", "r", stdin)){return false;}


    SetConsoleTitle(System::ConvertString(title).c_str());

    System::Log(Log::Level::INFO, "Console Create");

    return true;
}
