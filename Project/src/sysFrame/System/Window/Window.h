#pragma once
#include <string>
#include <Windows.h>

class Window{
    HINSTANCE hInstance_;
	HWND hWnd_;
	RECT windowRect_;

public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	~Window();
	bool Create(int clientWidth, int clientHeight, const std::wstring& titleName, const std::wstring& windowClassName);
	bool ProcessMessage();

    HINSTANCE GetInstanceHandle() const {
        return hInstance_;
    }

	HWND GetWindowHandle() const {
		return hWnd_;
	}
};

