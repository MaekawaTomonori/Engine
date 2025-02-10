#include "Input.h"

#include <cassert>

#include "System/System.h"
#include "Application/WinApp.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input* Input::instance = nullptr;
std::once_flag Input::flag;

void Input::Initialize(WinApp* winApp) {
	System::Log(Log::Level::INFO, "Input Enabled");

	winApp_ = winApp;

	HRESULT hr = DirectInput8Create(winApp->GetInstanceHandle(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(hr));

	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, nullptr);
	assert(SUCCEEDED(hr));

	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));

	hr = keyboard->SetCooperativeLevel(winApp->GetWindowHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(hr));

	hr = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	assert(SUCCEEDED(hr));

	hr = mouse->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(hr));

	hr = mouse->SetCooperativeLevel(winApp->GetWindowHandle(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	assert(SUCCEEDED(hr));
}

void Input::Update() {
	memcpy(preKey, keyState, sizeof(keyState));

	preMouseState = mouseState;

	keyboard->Acquire();

	keyboard->GetDeviceState(sizeof(keyState), keyState);

	mouse->Acquire();

	mouse->Poll();

	mouse->GetDeviceState(sizeof(mouseState), &mouseState);
}

bool Input::PushKey(BYTE key) const {
	return keyState[key];
}

bool Input::TriggerKey(BYTE key) const {
	return keyState[key] && !preKey[key];
}

bool Input::ReleaseKey(BYTE key) const {
	return !keyState[key] && preKey[key];
}

Vector2 Input::GetMousePosition() const {
	POINT point;

	GetCursorPos(&point);
	ScreenToClient(winApp_->GetWindowHandle(), &point);

	RECT client;
    GetClientRect(winApp_->GetWindowHandle(), &client);

    float scaleX = static_cast<float>(WinApp::CLIENT_WIDTH) / (client.right - client.left);
    float scaleY = static_cast<float>(WinApp::CLIENT_HEIGHT) / (client.bottom - client.top);

	Vector2 adjust {
		static_cast<float>(point.x) * scaleX,
		static_cast<float>(point.y) * scaleY,
	};

	return adjust;
}

bool Input::IsPushMouseButton(int mouseButton) const {
	if (mouseButton >= 0 && mouseButton <= 3) {
		return mouseState.rgbButtons[mouseButton];
	}
	return false;
}

bool Input::IsReleaseMouseButton(int mouseButton) const {
	if (mouseButton >= 0 && mouseButton <= 3) {
		return !mouseState.rgbButtons[mouseButton];
	}
	return false;
}

bool Input::IsTriggerPushMouseButton(int mouseButton) const {
	if (mouseButton >= 0 && mouseButton <= 3) {
		return mouseState.rgbButtons[mouseButton] && !preMouseState.rgbButtons[mouseButton];
	}
	return false;
}

bool Input::IsTriggerReleaseMouseButton(int mouseButton) const {
	if (mouseButton >= 0 && mouseButton <= 3) {
		return !mouseState.rgbButtons[mouseButton] && preMouseState.rgbButtons[mouseButton];
	}
	return false;
}

bool Input::IsPointInsideSprite(Sprite* sprite) const
{
	Vector2 mousePos = GetMousePosition();

	Vector2 spritePos = sprite->GetPosition();
	Vector2 spriteSize = sprite->GetSize();

	Vector2 anchorSpritePos = {
		spritePos.x - spriteSize.x * sprite->GetAnchorPoint().x,
		spritePos.y - spriteSize.y * sprite->GetAnchorPoint().y
	};

	if (mousePos.x > anchorSpritePos.x &&
		mousePos.x < anchorSpritePos.x + spriteSize.x &&
		mousePos.y > anchorSpritePos.y &&
		mousePos.y < anchorSpritePos.y + spriteSize.y) {
		return true;
	}
	return false;
}

Input* Input::GetInstance() {
	std::call_once(flag, Create);
	assert(instance);
	return instance;
}

void Input::Create() {
	instance = new Input();
	SingletonFinalizer::AddFinalizer(&Destroy);
}

void Input::Destroy() {
	delete instance;
	instance = nullptr;
	System::Log(Log::Level::INFO, "Input Disabled");
}
