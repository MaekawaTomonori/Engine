#pragma once
#include <memory>

#include "DirectX/Heap/Heap.h"

class SRVManager;
class WinApp;
class DirectXCommon;

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class ImGuiManager{
    //借りもの
    WinApp* winApp_ = nullptr;
    DirectXCommon* dxCommon_ = nullptr;
    SRVManager* srvManager_ = nullptr;

    uint32_t srvIndex_ = 0;

public:
	ImGuiManager(WinApp* winApp, DirectXCommon* dxCommon, SRVManager* srvManager) :winApp_(winApp), dxCommon_(dxCommon), srvManager_(srvManager) {}
    ~ImGuiManager();
	void Initialize();

    void Begin();
    void End();

    void Draw();
};

