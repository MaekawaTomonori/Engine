#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <wrl/client.h>
#include <windows.h>


class WinApp;

class Input{
    WinApp* winApp_ = nullptr;

	Microsoft::WRL::ComPtr<IDirectInput8> directInput = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard = nullptr;
	BYTE keyState[256] = {};
	BYTE preKey[256] = {};
public:
	void Initialize(WinApp* winApp);
	void Update();

	bool PushKey(BYTE key) const;
	bool TriggerKey(BYTE key) const;
	bool ReleaseKey(BYTE key) const;
};

