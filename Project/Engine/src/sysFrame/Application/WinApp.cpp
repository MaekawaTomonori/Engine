#include "WinApp.h"

#include <cassert>
#include <memory>

#include "System/System.h"
#include "System/Console/Console.h"

#pragma comment(lib, "winmm.lib")

void WinApp::Initialize(const std::string& title) {
    timeBeginPeriod(1);
    window_ = std::make_unique<Window>();
    if(!window_->Create(CLIENT_WIDTH, CLIENT_HEIGHT, System::ConvertString(title), L"Window")){
        System::Log(Logger::Level::ERR, "Window Creation Failed");
        assert(false);
        return;
    }
#ifdef _DEBUG
	if (!Console::Create(title)){
        System::Log(Logger::Level::ERR, "Console Creation Failed");
        assert(false);
        return;
    }
    window_->SetForeground();
#endif

    System::Log(Logger::Level::INFO, "WinApp Enabled");
}

void WinApp::Finalize() {
    //System::Logger(Logger::Level::INFO, "WinApp Disabled");
}

[[nodiscard]]
bool WinApp::ProcessMessage() const {
    return window_->ProcessMessage();
}

HINSTANCE WinApp::GetInstanceHandle() const {
    return window_->GetInstanceHandle();
}

HWND WinApp::GetWindowHandle() const {
	return window_->GetWindowHandle();
}
