#pragma once
#include <string>
#include <Windows.h>

class Window{
    HINSTANCE hInstance_ {};
	HWND hWnd_ {};
	RECT windowRect_ {};

	bool fullscreen_ = false;

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    static void Fullscreen(int width, int height);

	void Lock();
public:
	~Window();
	bool Create(int clientWidth, int clientHeight, const std::wstring& titleName, const std::wstring& windowClassName);
	bool ProcessMessage();

	void ToggleFullscreen();

    HINSTANCE GetInstanceHandle() const {
        return hInstance_;
    }

	HWND GetWindowHandle() const {
		return hWnd_;
	}

    void SetTitle(const std::string& title) const;
};

