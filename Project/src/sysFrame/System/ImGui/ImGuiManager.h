#pragma once
#include <memory>

#include "DirectX/Heap/Heap.h"

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

    std::shared_ptr<Heap> srv_;

public:
	ImGuiManager(WinApp* winApp, DirectXCommon* dxCommon) :winApp_(winApp), dxCommon_(dxCommon) {}
    ~ImGuiManager();
	void Initialize();

    void Begin();
    void End();

    void Draw();
};

