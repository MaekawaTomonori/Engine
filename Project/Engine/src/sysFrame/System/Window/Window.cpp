#include "Window.h"

//#include "imgui.h"
#include "imgui/imgui.h"
#include "System/System.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;

	switch (msg){
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::~Window() {
	CloseWindow(hWnd_);
}

bool Window::Create(int clientWidth, int clientHeight, const std::wstring& titleName, const std::wstring& windowClassName) {
	hInstance_ = GetModuleHandle(0);

	//window class. 
	WNDCLASS wc {};
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = windowClassName.c_str();
	wc.hInstance = hInstance_;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	if (!RegisterClass(&wc)){
		return false;
	}

	windowRect_ = {0, 0, clientWidth, clientHeight};

	AdjustWindowRect(&windowRect_, WS_OVERLAPPEDWINDOW, false);

	hWnd_ = CreateWindow(
		wc.lpszClassName,
		titleName.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect_.right - windowRect_.left,
		windowRect_.bottom - windowRect_.top,
		nullptr,
		nullptr,
		hInstance_,
		nullptr
	);

	if (hWnd_ == nullptr){
		return false;
	}


	ShowWindow(hWnd_, SW_SHOW);

	UpdateWindow(hWnd_);

	System::Log(Log::Level::INFO, "Window Created");

	return true;
}

[[nodiscard]]
bool Window::ProcessMessage() {
	MSG msg {};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
		if (msg.message == WM_QUIT){
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

void Window::SetForeground() const {
    SetForegroundWindow(hWnd_);
    SetFocus(hWnd_);
}
