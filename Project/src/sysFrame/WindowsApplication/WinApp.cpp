#include "WinApp.h"

#include <cassert>
#include <memory>

#include "System/System.h"
#include "System/Window/Window.h"
#include "DirectX/Heap/SRVManager.h"

void WinApp::Initialize(const std::string& title) {
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

HWND WinApp::GetWindowHandle() const {
	return window_->GetWindowHandle();
}
