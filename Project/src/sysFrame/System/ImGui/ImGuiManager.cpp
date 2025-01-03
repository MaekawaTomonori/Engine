#include "ImGuiManager.h"
#include "Application/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"

#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"

ImGuiManager::~ImGuiManager() {
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::Initialize(SRVManager* srv) {
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

}

void ImGuiManager::Begin() {
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::End() {
    ImGui::Render();
}

void ImGuiManager::Draw() {
    ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

    ComPtr<ID3D12DescriptorHeap> ppHeaps[] = {srv_->GetDescriptorHeap()};
    commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps->GetAddressOf());

    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());
}
