#include "Input.h"

#include <cassert>

#include "WindowsApplication/WinApp.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

void Input::Initialize(WinApp* winApp) {
    winApp_ = winApp;

    HRESULT hr = DirectInput8Create(winApp_->GetInstanceHandle(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
    assert(SUCCEEDED(hr));

    hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, nullptr);
    assert(SUCCEEDED(hr));

    hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
    assert(SUCCEEDED(hr));

    hr = keyboard->SetCooperativeLevel(winApp_->GetWindowHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    assert(SUCCEEDED(hr));
}

void Input::Update() {
    memcpy(preKey, keyState, sizeof(keyState));

    HRESULT hr;

	hr = keyboard->Acquire();

    assert(SUCCEEDED(hr));

    hr = keyboard->GetDeviceState(sizeof(keyState), keyState);
    assert(SUCCEEDED(hr));
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
