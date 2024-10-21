#pragma once
#include <memory>
#include <string>

class DirectXCommon;
class Window;

class WinApp{
private: //Variables
	std::shared_ptr<Window> window_;
	//std::shared_ptr<DirectXCommon> dxCommon_;
public:
	static const int32_t CLIENT_WIDTH = 1280;
    static const int32_t CLIENT_HEIGHT = 720;
public: //Functions
	void Initialize(const std::string& title);
	bool ProcessMessage() const;

};

