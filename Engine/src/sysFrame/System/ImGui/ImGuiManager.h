#pragma once
#include <Windows.h>
#include <memory>

#include "imgui/imgui.h"

class WinApp;
class DirectXCommon;
class SRVManager;


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class ImGuiManager{
    //借りもの
    WinApp* winApp_ = nullptr;
    DirectXCommon* dxCommon_ = nullptr;
    SRVManager* srv_ = nullptr;

public:
	ImGuiManager(WinApp* winApp, DirectXCommon* dxCommon) :winApp_(winApp), dxCommon_(dxCommon) {}
    ~ImGuiManager();
	void Initialize(SRVManager* srv);

    void Begin();
    void End();

    void Draw();
};

