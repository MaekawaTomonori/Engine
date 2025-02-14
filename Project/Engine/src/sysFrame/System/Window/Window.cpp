#include "Window.h"

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

void Window::Fullscreen(int width, int height) {
    DEVMODE dm {};
	memset(&dm, 0, sizeof(dm));

	if(!EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm)){
		MessageBox(nullptr, L"Could not enum display settings", L"err", MB_OK);
		return;
	}

    dm.dmPelsWidth = width;
    dm.dmPelsHeight = height;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

    int result = ChangeDisplaySettings(&dm, CDS_FULLSCREEN);

    if (result != DISP_CHANGE_SUCCESSFUL){
        MessageBox(nullptr, L"Could not change display settings", L"err", MB_OK);
		PostQuitMessage(0);
    }
}

void Window::Lock() const {
    LONG style = GetWindowLong(hWnd_, GWL_STYLE);
    style &= ~WS_SIZEBOX;
    style &= ~WS_MAXIMIZEBOX;
	SetWindowLong(hWnd_, GWL_STYLE, style);

	SetWindowPos(hWnd_, nullptr, 0, 0, 0, 0,
				 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
	);
}

Window::~Window() {
	CloseWindow(hWnd_);
}

bool Window::Create(const int clientWidth, const int clientHeight, const std::wstring& titleName, const std::wstring& windowClassName) {
	hInstance_ = GetModuleHandle(nullptr);

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

	//Fullscreen(clientWidth, clientHeight);

	hWnd_ = CreateWindow(
		wc.lpszClassName,
		titleName.c_str(),
		WS_OVERLAPPEDWINDOW,
		//WS_POPUP,
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
	SetWindowPos(hWnd_, HWND_TOP, 0, 0, clientWidth, clientHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOSIZE);

	Lock();

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

void Window::ToggleFullscreen() {
    fullscreen_ = !fullscreen_;
	static RECT rect;
    DWORD style = GetWindowLong(hWnd_, GWL_STYLE);
    if (fullscreen_){
        GetWindowRect(hWnd_, &rect);
        style &= ~WS_OVERLAPPEDWINDOW;
        style |= WS_POPUP;
        SetWindowLong(hWnd_, GWL_STYLE, style & ~WS_OVERLAPPED);

        HMONITOR hMonitor = MonitorFromWindow(hWnd_, MONITOR_DEFAULTTOPRIMARY);
        MONITORINFO monitorInfo = {sizeof(monitorInfo)};
		GetMonitorInfo(hMonitor, &monitorInfo);
    	SetWindowPos(hWnd_, HWND_TOP, 
					 monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, 
					 monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
					 monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
					 SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER
		);
    } else{
        style &= ~WS_POPUP;
        style |= WS_OVERLAPPEDWINDOW;
        SetWindowLong(hWnd_, GWL_STYLE, style);
	    SetWindowPos(hWnd_, HWND_TOP,
					 rect.left, rect.top, 
					 rect.right - rect.left,
					 rect.bottom - rect.top, 
					 SWP_SHOWWINDOW | SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER
		);
		Lock();
    }
}

void Window::SetTitle(const std::string& title) const {
	SetWindowText(hWnd_, System::ConvertString(title).c_str());
}
