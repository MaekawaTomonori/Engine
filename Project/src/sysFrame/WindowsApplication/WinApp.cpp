#include "WinApp.h"

#include <cassert>
#include <memory>

#include "System/System.h"
#include "DirectX/Heap/SRVManager.h"

#pragma comment(lib, "winmm.lib")

void WinApp::Initialize(const std::string& title) {
    timeBeginPeriod(1);
    window_ = std::make_shared<Window>();
    if(!window_->Create(CLIENT_WIDTH, CLIENT_HEIGHT, System::ConvertString(title), L"Window")){
        assert(0 && "Window Creation Failed");
        return;
    }
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
