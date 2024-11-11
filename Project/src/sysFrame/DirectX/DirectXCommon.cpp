#include "DirectXCommon.h"

#include <cassert>
#include <format>
#include <thread>

#include "WindowsApplication/WinApp.h"
#include "Heap/Heap.h"
#include "Pipeline/GraphicsPipeline.h"
#include "Shader/Shader.h"
#include "System/System.h"

bool DirectXCommon::Initialize(WinApp* winApp) {
    winApp_ = winApp;

    InitializeFixFPS();

    CoInitializeEx(0, COINIT_MULTITHREADED);
    CreateDebugLayer();
    CreateFactory();
    CreateDevice();
    CreateCommand();
    CreateSwapChain(winApp_->GetWindowHandle(), WinApp::CLIENT_WIDTH, WinApp::CLIENT_HEIGHT);
    CreateFence();
    CompileShader();
    CreateGraphicsPipeline();
    SettingGraphicsInfo();
    CreateDepthStencilView();
    //CreateShaderResourceView();
    return true;
}

void DirectXCommon::PreDraw() {
    UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();

    barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier_.Transition.pResource = swapChainBuffers_[backBufferIndex].Get();
    barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

    commandList_->ResourceBarrier(1, &barrier_);

    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap_->GetCPUHandle(0);
    commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex], false, &dsvHandle);

    float color[4] = {0.1f, 0.25f, 0.5f, 1.0f};
    commandList_->ClearRenderTargetView(rtvHandles_[backBufferIndex], color, 0, nullptr);
    commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);

    commandList_->RSSetViewports(1, &viewport_);
    commandList_->RSSetScissorRects(1, &scissorRect_);

    //DrawCall PSO
    graphicsPipeline_->DrawCall(commandList_.Get());
}

void DirectXCommon::PostDraw() {
    EndFrame();
}

void DirectXCommon::EndFrame() {
    HRESULT hr = S_OK;
    
    barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    
    commandList_->ResourceBarrier(1, &barrier_);

    hr = commandList_->Close();
    assert(SUCCEEDED(hr));

    ComPtr<ID3D12CommandList> cLists[] = {commandList_.Get()};
    commandQueue_->ExecuteCommandLists(1, cLists->GetAddressOf());
    
    swapChain_->Present(1, 0);
    
    WaitForCommandQueue();
    UpdateFixFPS();

    hr = commandAllocator_.Get()->Reset();
    assert(SUCCEEDED(hr));
    
    hr = commandList_.Get()->Reset(commandAllocator_.Get(), nullptr);
    assert(SUCCEEDED(hr));
}

ID3D12Resource* DirectXCommon::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {
    D3D12_HEAP_PROPERTIES properties {};
    properties.Type = D3D12_HEAP_TYPE_UPLOAD;

    //resource setting
    D3D12_RESOURCE_DESC desc {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Width = sizeInBytes;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.SampleDesc.Count = 1;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    ID3D12Resource* resource = nullptr;

#ifdef _DEBUG
    HRESULT hR =
#endif
        device->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));

    assert(SUCCEEDED(hR));

    return resource;
}

ID3D12Resource* DirectXCommon::CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height) {
    D3D12_RESOURCE_DESC desc {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.DepthOrArraySize = 1;
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.SampleDesc.Count = 1;
    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_HEAP_PROPERTIES properties {};
    properties.Type = D3D12_HEAP_TYPE_DEFAULT;

    D3D12_CLEAR_VALUE clearValue {};
    clearValue.DepthStencil.Depth = 1.f;
    clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    ID3D12Resource* resource = nullptr;
    HRESULT hr = device->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, IID_PPV_ARGS(&resource));
    assert(SUCCEEDED(hr));

    return resource;
}

ID3D12DescriptorHeap* DirectXCommon::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT count,
	bool shaderVisible) const {
    ID3D12DescriptorHeap* heap = nullptr;

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = type;
    desc.NumDescriptors = count;
    desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    HRESULT hr = device_->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
    assert(SUCCEEDED(hr));
    return heap;
}

DirectXCommon::~DirectXCommon() {
    CoUninitialize();
}

void DirectXCommon::CreateDebugLayer() {
    #ifdef _DEBUG
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_)))){
        debugController_->EnableDebugLayer();
        debugController_->SetEnableGPUBasedValidation(TRUE);
    }
    //leakChecker_ = std::make_shared<D3DResourceLeakChecker>();
    #endif
}

void DirectXCommon::CreateFactory() {
    HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&factory_));
    assert(SUCCEEDED(hr));
}

void DirectXCommon::CreateDevice() {
    HRESULT hr = S_OK;

    ComPtr<IDXGIAdapter4> useAdapter = nullptr;
    for (UINT i = 0; factory_->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND; ++i){
        DXGI_ADAPTER_DESC3 adapterDesc {};
        hr = useAdapter->GetDesc3(&adapterDesc);
        assert(SUCCEEDED(hr));

        if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)){
            System::Log(std::format(L"Use Adapter:{}\n", adapterDesc.Description));
            break;
        }
        useAdapter = nullptr;
    }
    assert(SUCCEEDED(useAdapter != nullptr));

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_12_2,
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
    };
    const char* featureLevelStrings[] = {
        "12.2",
        "12.1",
        "12.0"
    };

    for (size_t i = 0; i < _countof(featureLevels); ++i){
        hr = D3D12CreateDevice(useAdapter.Get(), featureLevels[i], IID_PPV_ARGS(device_.GetAddressOf()));
        if (SUCCEEDED(hr)){
            System::Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
            break;
        }
    }

    assert(device_ != nullptr);
    System::Log("Complete creation\n");

    #ifdef _DEBUG
    ComPtr<ID3D12InfoQueue> infoQueue;
    if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))){
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
    }
    D3D12_MESSAGE_ID denyIds[] = {
        D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
    };

    D3D12_MESSAGE_SEVERITY severities[] = {
        D3D12_MESSAGE_SEVERITY_INFO
    };
    D3D12_INFO_QUEUE_FILTER filter {};
    filter.DenyList.NumIDs = _countof(denyIds);
    filter.DenyList.pIDList = denyIds;
    filter.DenyList.NumSeverities = _countof(severities);
    filter.DenyList.pSeverityList = severities;
    infoQueue->PushStorageFilter(&filter);
    #endif
}

void DirectXCommon::CreateCommand() {
    HRESULT hr = S_OK;
    hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
    assert(SUCCEEDED(hr));

    hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
    assert(SUCCEEDED(hr));

    D3D12_COMMAND_QUEUE_DESC cQueueDesc {};
    cQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    cQueueDesc.NodeMask = 0;
    cQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    cQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    
    hr = device_->CreateCommandQueue(&cQueueDesc, IID_PPV_ARGS(&commandQueue_));
    assert(SUCCEEDED(hr));
}

void DirectXCommon::CreateSwapChain(HWND hwnd, int width, int height) {
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc {};
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    HRESULT hr = factory_->CreateSwapChainForHwnd(commandQueue_.Get(), hwnd, &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.ReleaseAndGetAddressOf()));
    assert(SUCCEEDED(hr));

    
    hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainBuffers_[0]));
    assert(SUCCEEDED(hr));
    hr = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainBuffers_[1]));
    assert(SUCCEEDED(hr));
    
    //RTV
    rtvHeap_ = std::make_shared<Heap>();
    rtvHeap_->Create(device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);

    rtvHandles_[0] = rtvHeap_->GetCPUHandle(0);
    rtvHandles_[1] = rtvHeap_->GetCPUHandle(1);


    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc {};
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    
    device_->CreateRenderTargetView(swapChainBuffers_[0].Get(), &rtvDesc, rtvHandles_[0]);
    device_->CreateRenderTargetView(swapChainBuffers_[1].Get(), &rtvDesc, rtvHandles_[1]);
}

void DirectXCommon::CreateFence() {
    //
    HRESULT hr = S_OK;
    hr = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
    assert(SUCCEEDED(hr));
}

void DirectXCommon::CompileShader() {
    shader_ = std::make_shared<Shader>();
    shader_->Create();
}

void DirectXCommon::CreateGraphicsPipeline() {
    graphicsPipeline_ = std::make_shared<GraphicsPipeline>();
    graphicsPipeline_->Create(device_.Get(), shader_->GetVertexShader(), shader_->GetPixelShader());
}

void DirectXCommon::SettingGraphicsInfo() {
    viewport_.Width = WinApp::CLIENT_WIDTH;
    viewport_.Height = WinApp::CLIENT_HEIGHT;

    viewport_.TopLeftX = 0;
    viewport_.TopLeftY = 0;
    viewport_.MinDepth = 0;
    viewport_.MaxDepth = 1.f;

    scissorRect_.left = 0;
    scissorRect_.right = WinApp::CLIENT_WIDTH;
    scissorRect_.top = 0;
    scissorRect_.bottom = WinApp::CLIENT_HEIGHT;
}

void DirectXCommon::CreateDepthStencilView() {
    depthStencilResource_.Attach(DirectXCommon::CreateDepthStencilTextureResource(device_.Get(), WinApp::CLIENT_WIDTH, WinApp::CLIENT_HEIGHT));

    dsvHeap_ = std::make_shared<Heap>();
    dsvHeap_->Create(device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);

    dsvDesc_.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc_.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

    device_->CreateDepthStencilView(depthStencilResource_.Get(), &dsvDesc_, dsvHeap_->GetCPUHandle(0));
}

//void DirectXCommon::CreateShaderResourceView() {
//    srv_ = std::make_shared<Heap>();
//    srv_->Create(device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);
//}
//
void DirectXCommon::WaitForCommandQueue() {
    commandQueue_->Signal(fence_.Get(), ++fenceValue_);

    if (fence_->GetCompletedValue() < fenceValue_){
        HANDLE fenceEvent = CreateEvent(nullptr, false, false, nullptr);
        assert(fenceEvent != nullptr);

        fence_->SetEventOnCompletion(fenceValue_, fenceEvent);

        WaitForSingleObject(fenceEvent, INFINITE);

        CloseHandle(fenceEvent);
    }
}

void DirectXCommon::InitializeFixFPS() {
    reference_ = std::chrono::steady_clock::now();
}

void DirectXCommon::UpdateFixFPS() {
	constexpr std::chrono::microseconds MIN_TIME(static_cast<uint64_t>(1e6 / 60));
    constexpr std::chrono::microseconds MIN_CHECK_TIME(static_cast<uint64_t>(1e6 / 65));

    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::microseconds elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

    if (elapsedTime < MIN_CHECK_TIME) {
	    while (std::chrono::steady_clock::now() - reference_ < MIN_TIME) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
	    }
    }

    reference_ = std::chrono::steady_clock::now();
}
