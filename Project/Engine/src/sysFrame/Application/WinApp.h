#pragma once
#include <memory>
#include <string>

#include "System/Console/Console.h"
#include "System/Window/Window.h"

class WinApp{
private: //Variables
	std::unique_ptr<Window> window_;
	std::unique_ptr<Console> console_;
public:
	static const int32_t CLIENT_WIDTH = 1280;
    static const int32_t CLIENT_HEIGHT = 720;
public: //Functions
	void Initialize(const std::string& title);
	void Finalize();
	bool ProcessMessage() const;

    HINSTANCE GetInstanceHandle() const;
    HWND GetWindowHandle() const;
};

