#pragma once
#include <string>
#include <Windows.h>

class Window{
    HINSTANCE hInstance_ {};
	HWND hWnd_ {};
	RECT windowRect_ {};

    static ULONG_PTR gdiplusToken;
private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static void DisplayLogo(HWND hWnd);

	void Titlebar(bool stats);
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

    void SetForeground() const;
	void EnableTitlebar() ;
	void DisableTitlebar();
};

