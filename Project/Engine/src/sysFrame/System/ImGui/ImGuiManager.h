#pragma once
#include <deque>
#include <memory>
#include <Windows.h>

#include "ImGuiCommand.h"
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

    std::unique_ptr<ImGuiCommand> command_;

    std::string logPath_;
    std::deque<std::string> logs_;
    bool logAutoScroll_ = true;

    static ImGuiManager* instance_;
    static std::once_flag onceFlag_;

public:
    static ImGuiManager* GetInstance();

	void Initialize(WinApp* winApp, DirectXCommon* dxCommon, SRVManager* srv);

    void AddCommand(void* ptr, const std::function<void()>& command) const;

    void Begin();
    void End();

    void Draw();

private:
	ImGuiManager() = default;
    ~ImGuiManager();
    ImGuiManager(const ImGuiManager&) = delete;
    ImGuiManager& operator=(const ImGuiManager&) = delete;
	static void Create();
    static void Destroy();

    void DockingSpace();
    void DisplayLog();
};

