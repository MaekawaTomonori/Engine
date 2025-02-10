#pragma once
#include <memory>
#include <string>

#include "System/Window/Window.h"

class WinApp{
private: //Variables
	std::shared_ptr<Window> window_;
public:
	static const int32_t CLIENT_WIDTH = 1280;
    static const int32_t CLIENT_HEIGHT = 720;
public: //Functions
	void Initialize(const std::string& title);
	void Finalize();
	bool ProcessMessage() const;

    HINSTANCE GetInstanceHandle() const;
    HWND GetWindowHandle() const;
	void ToggleFullscreen() const;
	void SetTitle(const std::string& title) const;
};

