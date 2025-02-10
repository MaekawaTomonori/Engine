#include "ImGuiManager.h"
#include "Application/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"

#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

ImGuiManager* ImGuiManager::instance_ = nullptr;
std::once_flag ImGuiManager::onceFlag_;

ImGuiManager::~ImGuiManager() {
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

ImGuiManager* ImGuiManager::GetInstance() {
    std::call_once(onceFlag_, Create);
    assert(instance_);
    return instance_;
}

void ImGuiManager::Create() {
    instance_ = new ImGuiManager();
    SingletonFinalizer::AddFinalizer(&Destroy);
}

void ImGuiManager::Destroy() {
    delete instance_;
    instance_ = nullptr;
}

void ImGuiManager::Initialize(WinApp* winApp, DirectXCommon* dxCommon, SRVManager* srv) {
    winApp_ = winApp;
    dxCommon_ = dxCommon;
    srv_ = srv;
	uint32_t index = srv_->Allocate();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(winApp_->GetWindowHandle());

    ImGui_ImplDX12_Init(
        dxCommon_->GetDevice().Get(),
        static_cast<int>(dxCommon_->GetBackBufferCount()),
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        srv_->GetDescriptorHeap(),
        srv_->GetCPUHandle(index),
        srv_->GetGPUHandle(index)
    );

    command_ = std::make_unique<ImGuiCommand>();
}

void ImGuiManager::AddCommand(void* ptr, const std::function<void()>& command) const {
    command_->AddCommand(ptr, command);
}

void ImGuiManager::Begin() {
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::End() const {
    command_->Update();
    ImGui::Render();
}

void ImGuiManager::Draw() {
    ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

    ComPtr<ID3D12DescriptorHeap> ppHeaps[] = {srv_->GetDescriptorHeap()};
    commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps->GetAddressOf());

    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());
}
