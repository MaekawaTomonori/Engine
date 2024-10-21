#include "Window.h"

#include "System/System.h"

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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

bool Window::Create(int clientWidth, int clientHeight, const std::wstring& titleName,
					const std::wstring& windowClassName) {
	HINSTANCE hInstance = GetModuleHandle(0);

	//window class. 
	WNDCLASS wc {};
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = windowClassName.c_str();
	wc.hInstance = hInstance;
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
		clientWidth,
		clientHeight,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (hWnd_ == nullptr){
		return false;
	}


	ShowWindow(hWnd_, SW_SHOW);

	UpdateWindow(hWnd_);

	System::Log("Window Created\n");

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
