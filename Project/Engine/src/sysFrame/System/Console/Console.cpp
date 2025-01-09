#include "Console.h"

#include <iostream>
#include <Windows.h>

#include "System/System.h"

[[nodiscard]]
bool Console::Create(const std::string& title) {
    if(AllocConsole() == 0){
        return false;
    }

    FILE* fp = nullptr;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

    SetConsoleTitle(System::ConvertString(title).c_str());

    std::wcout << L"Console Initialized" << std::endl;
    return true;
}
