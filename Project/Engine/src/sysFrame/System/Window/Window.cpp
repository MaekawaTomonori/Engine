#include "Window.h"

#pragma comment(lib, "Gdiplus.lib")
#include <ole2.h>
#include <gdiplus.h>

//#include "imgui.h"
#include "DirectXTex.h"
#include "imgui/imgui.h"
#include "System/System.h"

ULONG_PTR Window::gdiplusToken;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;

	switch (msg){
	case WM_DESTROY:
		PostQuitMessage(0);
	case WM_PAINT:
		DisplayLogo(hWnd);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::DisplayLogo(HWND hWnd) {
	PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	Gdiplus::Graphics graphics(hdc);

	Gdiplus::Image image(L"Assets/Resources/logo.png");
    if (image.GetLastStatus() != Gdiplus::Ok){
        System::Log(Logger::Level::ERR, "Failed to Load Image");
		assert(false);
        return;
    }

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	int imgW = static_cast<INT>(image.GetWidth());
    int imgH = static_cast<INT>(image.GetHeight());

	int x = static_cast<INT>((clientRect.right - clientRect.left) - imgW) / 2;
	int y = static_cast<INT>((clientRect.bottom - clientRect.top) - imgH) / 2;

    Gdiplus::Rect destRect(x, y, imgW, imgH);
	graphics.DrawImage(&image, destRect);

	EndPaint(hWnd, &ps);

	System::Log(Logger::Level::INFO, "Logo Displayed");
}

void Window::Titlebar(bool stats) {
    LONG style = GetWindowLong(hWnd_, GWL_STYLE);
    style |= stats ? WS_CAPTION : ~WS_CAPTION;
    SetWindowLong(hWnd_, GWL_STYLE, style);
	AdjustWindowRect(&windowRect_, style, false);
    SetWindowPos(hWnd_, nullptr, windowRect_.left, windowRect_.top, windowRect_.right-windowRect_.left, windowRect_.bottom - windowRect_.top, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

Window::~Window() {
	Gdiplus::GdiplusShutdown(gdiplusToken);
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

	if (!hWnd_){
        DWORD error = GetLastError();
		System::Log(Logger::Level::ERR, std::format("Window Creation Error : {}", error));
        return false;
	}

	ShowWindow(hWnd_, SW_SHOW);

	UpdateWindow(hWnd_);

	System::Log(Logger::Level::INFO, "Window Created");

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

void Window::EnableTitlebar() {
    Titlebar(true);
}

void Window::DisableTitlebar() {
    Titlebar(false);
}
