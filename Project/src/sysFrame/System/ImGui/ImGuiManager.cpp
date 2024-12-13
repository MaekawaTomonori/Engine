#include "ImGuiManager.h"

#include "WindowsApplication/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"


void ImGuiManager::Initialize(SRVManager* srv) {
#ifdef _DEBUG
    srvManager_ = srv;
    srvIndex_ = srvManager_->Allocate();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(winApp_->GetWindowHandle());

    auto dxc = dxCommon_.lock();
    if (!dxc){
        return;
    }

    ImGui_ImplDX12_Init(
        dxc->GetDevice().Get(),
        static_cast<int>(dxc->GetBackBufferCount()),
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        srvManager_->GetDescriptorHeap(),
        srvManager_->GetCPUHandle(srvIndex_),
        srvManager_->GetGPUHandle(srvIndex_)
    );

    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.f;
    io.Fonts->AddFontDefault();
#endif
}

void ImGuiManager::Begin() {
#ifdef _DEBUG
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
#endif
}

void ImGuiManager::End() {
#ifdef _DEBUG
    ImGui::Render();
#endif
}

void ImGuiManager::Draw() const {
#ifdef _DEBUG
    ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_.lock()->GetCommandList();

    ComPtr<ID3D12DescriptorHeap> ppHeaps[] = {srvManager_->GetDescriptorHeap()};
    commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps->GetAddressOf());

    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());
#endif
}

void ImGuiManager::Finalize() {
#ifdef _DEBUG
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
#endif
}
