#include "ImGuiManager.h"

#include "WindowsApplication/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"

ImGuiManager::~ImGuiManager() {
#ifdef DEBUG
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
#endif
}

void ImGuiManager::Initialize() {
#ifdef DEBUG
    srvIndex_ = srvManager_->Allocate();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(winApp_->GetWindowHandle());

    ImGui_ImplDX12_Init(
        dxCommon_->GetDevice(),
        static_cast<int>(dxCommon_->GetBackBufferCount()),
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        srvManager_->GetDescriptorHeap(),
        srvManager_->GetCPUHandle(srvIndex_),
        srvManager_->GetGPUHandle(srvIndex_)
    );
#endif
}

void ImGuiManager::Begin() {
#ifdef DEBUG
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
#endif
}

void ImGuiManager::End() {
#ifdef DEBUG
    ImGui::Render();
#endif
}

void ImGuiManager::Draw() {
#ifdef DEBUG
    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

    ID3D12DescriptorHeap* ppHeaps[] = {srvManager_->GetDescriptorHeap()};
    commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
#endif
}
