#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <mutex>
#include <wrl/client.h>
#include <windows.h>
#include "sysFrame/Application/WinApp.h"
#include "System/Math/Vector2.h"
#include "Object/Sprite/Sprite.h"

class WinApp;

class Input{

public:
	enum MouseButton {
		Left,
		Right,
		Middle
	};

//Member
private:
	WinApp* winApp_;
	Microsoft::WRL::ComPtr<IDirectInput8> directInput = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse = nullptr;
	BYTE keyState[256] = {};
	BYTE preKey[256] = {};
	DIMOUSESTATE mouseState;
	DIMOUSESTATE preMouseState;
public:
	void Initialize(WinApp* winApp);
	void Update();

	bool PushKey(BYTE key) const;
	bool TriggerKey(BYTE key) const;
	bool ReleaseKey(BYTE key) const;

	Vector2 GetMousePosition() const;
	bool IsPushMouseButton(int mouseButton) const;
	bool IsReleaseMouseButton(int mouseButton) const;
	bool IsTriggerPushMouseButton(int mouseButton) const;
	bool IsTriggerReleaseMouseButton(int mouseButton) const;

	bool IsPointInsideSprite(Sprite* sprite) const;

//Static
private:
	Input() = default;
	~Input() = default;
	static Input* instance;
    static std::once_flag flag;

	static void Create();
	static void Destroy();
public:
	static Input* GetInstance();
};

