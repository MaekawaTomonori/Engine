#include "Input.h"

#include <cassert>

#include "System/System.h"
#include "Application/WinApp.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input* Input::instance = nullptr;
std::once_flag Input::flag;

void Input::Initialize(const WinApp* winApp) {
    System::Log(Log::Level::INFO, "Input Enabled");

    HRESULT hr = DirectInput8Create(winApp->GetInstanceHandle(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
    assert(SUCCEEDED(hr));

    hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, nullptr);
    assert(SUCCEEDED(hr));

    hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
    assert(SUCCEEDED(hr));

    hr = keyboard->SetCooperativeLevel(winApp->GetWindowHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    assert(SUCCEEDED(hr));
}

void Input::Update() {
    memcpy(preKey, keyState, sizeof(keyState));

	keyboard->Acquire();

    keyboard->GetDeviceState(sizeof(keyState), keyState);
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
