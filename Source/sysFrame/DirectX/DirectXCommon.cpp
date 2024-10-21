#include "DirectXCommon.h"

#include <format>

#include "System/System.h"

bool DirectXCommon::Initialize(const std::shared_ptr<WinApp>& winApp) {
    winApp_ = winApp;

    CreateDebugLayer();
    CreateFactory();
    CreateDevice();
    //CreateCommand();
    //CreateSwapChain(hwnd, width, height);
    //CreateFence();
    //CreateDxc();
    //CompileShaders();
    //CreateGraphicsPipeline();
    //SettingGraphicsInfo();
    //CreateShaderResourceView();
    return true;
}

//void DirectXCommon::PreDraw() {
//    UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();
//
//    barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
//    barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
//    barrier_.Transition.pResource = swapChainBuffers_[backBufferIndex].Get();
//    barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
//    barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
//
//    cList_->ResourceBarrier(1, &barrier_);
//
//    cList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex], false, nullptr);
//
//    float color[4] = {0.1f, 0.25f, 0.5f, 1.0f};
//    cList_->ClearRenderTargetView(rtvHandles_[backBufferIndex], color, 0, nullptr);
//
//    cList_->RSSetViewports(1, &viewport_);
//    cList_->RSSetScissorRects(1, &scissorRect_);
//
//    //DrawCall PSO
//    graphicsPipeline_->DrawCall(cList_.Get());
//}
//
//void DirectXCommon::PostDraw() {
//}
//
//void DirectXCommon::EndFrame() {
//    HRESULT hr = S_OK;
//
//    barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
//    barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
//
//    cList_->ResourceBarrier(1, &barrier_);
//
//    hr = cList_->Close();
//    assert(SUCCEEDED(hr));
//
//    ComPtr<ID3D12CommandList> cLists[] = {cList_.Get()};
//    cQueue_->ExecuteCommandLists(1, cLists->GetAddressOf());
//
//    swapChain_->Present(1, 0);
//
//    WaitForCommandQueue();
//
//    hr = cAllocator_.Get()->Reset();
//    assert(SUCCEEDED(hr));
//
//    hr = cList_.Get()->Reset(cAllocator_.Get(), nullptr);
//    assert(SUCCEEDED(hr));
//}

ID3D12Resource* DirectXCommon::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {
    D3D12_HEAP_PROPERTIES uploadHeapProperties {};
    uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

    //resource setting
    D3D12_RESOURCE_DESC materialResourceDesc {};
    materialResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    materialResourceDesc.Width = sizeInBytes;
    materialResourceDesc.Height = 1;
    materialResourceDesc.DepthOrArraySize = 1;
    materialResourceDesc.MipLevels = 1;
    materialResourceDesc.SampleDesc.Count = 1;
    materialResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    ID3D12Resource* materialResource = nullptr;

    #ifdef _DEBUG
    HRESULT hR =
        #endif

        device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &materialResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&materialResource));

    assert(SUCCEEDED(hR));

    return materialResource;
}

DirectXCommon::~DirectXCommon() {
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

//void DirectXCommon::CreateCommand() {
//    HRESULT hr = S_OK;
//    hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cAllocator_));
//    assert(SUCCEEDED(hr));
//
//    hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cAllocator_.Get(), nullptr, IID_PPV_ARGS(&cList_));
//    assert(SUCCEEDED(hr));
//
//    D3D12_COMMAND_QUEUE_DESC cQueueDesc {};
//    cQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
//    cQueueDesc.NodeMask = 0;
//    cQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
//    cQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
//
//    hr = device_->CreateCommandQueue(&cQueueDesc, IID_PPV_ARGS(&cQueue_));
//    assert(SUCCEEDED(hr));
//}
//
//void DirectXCommon::CreateSwapChain(HWND hwnd, int width, int height) {
//    DXGI_SWAP_CHAIN_DESC1 swapChainDesc {};
//    swapChainDesc.Width = width;
//    swapChainDesc.Height = height;
//    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//    swapChainDesc.SampleDesc.Count = 1;
//    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//    swapChainDesc.BufferCount = 2;
//    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
//    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
//
//    HRESULT hr = factory_->CreateSwapChainForHwnd(cQueue_.Get(), hwnd, &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.ReleaseAndGetAddressOf()));
//    assert(SUCCEEDED(hr));
//
//    //RTV
//    rtvHeap_ = std::make_shared<Heap>();
//    rtvHeap_->Create(device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
//
//    hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainBuffers_[0]));
//    assert(SUCCEEDED(hr));
//    hr = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainBuffers_[1]));
//    assert(SUCCEEDED(hr));
//
//    rtvHandles_[0] = rtvHeap_->GetCPUHandle(0);
//    rtvHandles_[1] = rtvHeap_->GetCPUHandle(1);
//
//
//    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc {};
//    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
//
//    device_->CreateRenderTargetView(swapChainBuffers_[0].Get(), &rtvDesc, rtvHandles_[0]);
//    device_->CreateRenderTargetView(swapChainBuffers_[1].Get(), &rtvDesc, rtvHandles_[1]);
//}
//
//void DirectXCommon::CreateFence() {
//    //
//    HRESULT hr = S_OK;
//    hr = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
//    assert(SUCCEEDED(hr));
//}
//
//void DirectXCommon::CreateDxc() {
//    HRESULT hr = S_OK;
//    hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
//    assert(SUCCEEDED(hr));
//    hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
//    assert(SUCCEEDED(hr));
//
//    hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
//    assert(SUCCEEDED(hr));
//}
//
//void DirectXCommon::CompileShaders() {
//    vertexShader_.Attach(Shader::Compile(L"Assets/Shaders/", L"Object3d.VS.hlsl", L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get()));
//    pixelShader_.Attach(Shader::Compile(L"Assets/Shaders/", L"Object3d.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get()));
//}
//
//void DirectXCommon::CreateGraphicsPipeline() {
//    graphicsPipeline_ = std::make_shared<GraphicsPipeline>();
//    graphicsPipeline_->Create(device_.Get(), vertexShader_.Get(), pixelShader_.Get());
//}
//
//void DirectXCommon::SettingGraphicsInfo() {
//    viewport_.Width = CLIENT_WIDTH;
//    viewport_.Height = CLIENT_HEIGHT;
//
//    viewport_.TopLeftX = 0;
//    viewport_.TopLeftY = 0;
//    viewport_.MinDepth = 0;
//    viewport_.MaxDepth = 1.f;
//
//    scissorRect_.left = 0;
//    scissorRect_.right = CLIENT_WIDTH;
//    scissorRect_.top = 0;
//    scissorRect_.bottom = CLIENT_HEIGHT;
//}
//
//void DirectXCommon::CreateShaderResourceView() {
//    srv_ = std::make_shared<Heap>();
//    srv_->Create(device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);
//}
//
//void DirectXCommon::WaitForCommandQueue() {
//    ++fenceValue_;
//
//    cQueue_->Signal(fence_.Get(), fenceValue_);
//
//    if (fence_->GetCompletedValue() < fenceValue_){
//        HANDLE fenceEvent = CreateEvent(nullptr, false, false, nullptr);
//        assert(fenceEvent != nullptr);
//
//        fence_->SetEventOnCompletion(fenceValue_, fenceEvent);
//
//        WaitForSingleObject(fenceEvent, INFINITE);
//
//        CloseHandle(fenceEvent);
//    }
//}