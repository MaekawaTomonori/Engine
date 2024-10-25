#include "ImGuiManager.h"

#include <memory>

#include "Application/WinApp.h"

ImGuiManager::~ImGuiManager() {
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::Initialize() {
    srv_ = std::make_shared<Heap>();
    srv_->Create(dxCommon_->GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1, true);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(winApp_->GetWindowHandle());

    ImGui_ImplDX12_Init(
        dxCommon_->GetDevice(),
        static_cast<int>(dxCommon_->GetBackBufferCount()),
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        srv_->GetHeap(),
        srv_->GetCPUHandle(0),
        srv_->GetGPUHandle(0)
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
    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

    ID3D12DescriptorHeap* ppHeaps[] = {srv_->GetHeap()};
    commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}
