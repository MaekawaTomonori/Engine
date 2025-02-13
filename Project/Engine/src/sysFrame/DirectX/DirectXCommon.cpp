#include "DirectXCommon.h"

#include <cassert>
#include <d3dcompiler.h>
#include <format>

#include "d3dx12.h"
#include "Application/WinApp.h"
#include "Heap/Heap.h"
#include "Heap/SRVManager.h"
#include "Shader/Shader.h"
#include "System/System.h"
#include "System/ImGui/ImGuiManager.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

bool DirectXCommon::Initialize(const WinApp* winApp) {
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    CreateDebugLayer();
    CreateFactory();
    CreateDevice();
    CreateCommand();
    CreateSwapChain(winApp->GetWindowHandle(), WinApp::CLIENT_WIDTH, WinApp::CLIENT_HEIGHT);
    CreateFence();
    SettingGraphicsInfo();
    CreateDepthStencilView();
    InitializeFixFPS();


    backColor_ = {0.1f, 0.25f, 0.5f, 1.0f};
    System::Log(Log::Level::INFO, "DirectXCommon Enabled");

    return true;
}

void DirectXCommon::EnablePP(SRVManager* srv) {
    srvManager_ = srv;
    CreatePostProcessResource();
    CreateScreenPipeline();
}

void DirectXCommon::Finalize() {
    CoUninitialize();
    //System::Log(Log::Level::INFO, "DirectXCommon Disabled");
}

ComPtr<ID3D12Resource> DirectXCommon::CreateBufferResource(const ComPtr<ID3D12Device>& device, size_t sizeInBytes) {
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

ComPtr<ID3D12Resource> DirectXCommon::CreateDepthStencilTextureResource(const ComPtr<ID3D12Device>& device, int32_t width, int32_t height) {
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

ComPtr<ID3D12DescriptorHeap> DirectXCommon::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT count,
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
            System::Log(/*Log::Level::INFO,*/std::format(L"Use Adapter:{}", adapterDesc.Description));
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
            System::Log(/*Log::Level::INFO, */std::format("FeatureLevel : {}", featureLevelStrings[i]));
            break;
        }
    }

    assert(device_ != nullptr);
    System::Log(/*Log::Level::INFO,*/"Complete creation!");

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

    System::Log(/*Log::Level::INFO, */"CommandAllocator Created");

    hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
    assert(SUCCEEDED(hr));

    System::Log(/*Log::Level::INFO,*/ "CommandList Created");

    D3D12_COMMAND_QUEUE_DESC cQueueDesc {};
    cQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    cQueueDesc.NodeMask = 0;
    cQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    cQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    
    hr = device_->CreateCommandQueue(&cQueueDesc, IID_PPV_ARGS(&commandQueue_));
    assert(SUCCEEDED(hr));

    System::Log(/*Log::Level::INFO, */"CommandQueue Created");
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

    System::Log(/*Log::Level::INFO, */"SwapChain Created");
    
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

void DirectXCommon::SettingGraphicsInfo() {
    viewport_.TopLeftX = 0;
    viewport_.TopLeftY = 0;
    viewport_.Width = WinApp::CLIENT_WIDTH;
    viewport_.Height = WinApp::CLIENT_HEIGHT;

    viewport_.MinDepth = 0;
    viewport_.MaxDepth = 1.f;

    scissorRect_.left = 0;
    scissorRect_.right = WinApp::CLIENT_WIDTH;
    scissorRect_.top = 0;
    scissorRect_.bottom = WinApp::CLIENT_HEIGHT;
}

void DirectXCommon::CreateDepthStencilView() {
    depthStencilResource_.Attach(CreateDepthStencilTextureResource(device_, WinApp::CLIENT_WIDTH, WinApp::CLIENT_HEIGHT).Get());

    dsvHeap_ = std::make_shared<Heap>();
    dsvHeap_->Create(device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);

    dsvDesc_.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc_.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

    device_->CreateDepthStencilView(depthStencilResource_.Get(), &dsvDesc_, dsvHeap_->GetCPUHandle(0));

    System::Log(/*Log::Level::INFO, */"DepthStencilView Created");
}

void DirectXCommon::CreatePostProcessResource() {
	const auto& bbf = swapChainBuffers_[0];
    auto resDesc = bbf->GetDesc();

    resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

    D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);


    D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, &backColor_.x);
    auto result = device_->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        &clearValue,
        IID_PPV_ARGS(canvasResource_.ReleaseAndGetAddressOf())
    );
    assert(SUCCEEDED(result));

    float black[4] = {0, 0, 0, 1};
    D3D12_CLEAR_VALUE clearValueBlack = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, black);
	result = device_->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        &clearValueBlack,
        IID_PPV_ARGS(rootColorResource_.ReleaseAndGetAddressOf())
    );
    assert(SUCCEEDED(result));

	result = device_->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        &clearValueBlack,
        IID_PPV_ARGS(bloomResource_.ReleaseAndGetAddressOf())
    );
    assert(SUCCEEDED(result));

	result = device_->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        &clearValueBlack,
        IID_PPV_ARGS(blurResource_.ReleaseAndGetAddressOf())
    );
    resDesc.Width = 720;
	assert(SUCCEEDED(result));


	auto rtvHeapDesc = rtvHeap_->GetDescriptorHeap()->GetDesc();
    rtvHeapDesc.NumDescriptors = 4;
    result = device_->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(rtvForPP_.ReleaseAndGetAddressOf()));
    assert(SUCCEEDED(result));

    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

    auto handle = rtvForPP_->GetCPUDescriptorHandleForHeapStart();
    device_->CreateRenderTargetView(canvasResource_.Get(), &rtvDesc, handle);

    handle.ptr += device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    device_->CreateRenderTargetView(rootColorResource_.Get(), &rtvDesc, handle);

    handle.ptr += device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    device_->CreateRenderTargetView(blurResource_.Get(), &rtvDesc, handle);

	handle.ptr += device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    device_->CreateRenderTargetView(bloomResource_.Get(), &rtvDesc, handle);

    indexes_[0] = srvManager_->Allocate();
    srvManager_->CreateSRVforTexture2D(indexes_[0], canvasResource_.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, 1);

    indexes_[1] = srvManager_->Allocate();
    srvManager_->CreateSRVforTexture2D(indexes_[1], rootColorResource_.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, 1);

    indexes_[2] = srvManager_->Allocate();
    srvManager_->CreateSRVforTexture2D(indexes_[2], blurResource_.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, 1);

    indexes_[3] = srvManager_->Allocate();
    srvManager_->CreateSRVforTexture2D(indexes_[3], bloomResource_.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, 1);
}

void DirectXCommon::CreateScreenPipeline() {
    D3D12_DESCRIPTOR_RANGE range {};
    range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    range.BaseShaderRegister = 0;
    range.NumDescriptors = 4;

    D3D12_ROOT_PARAMETER rp[1] {};
    rp[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rp[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rp[0].DescriptorTable.pDescriptorRanges = &range;
    rp[0].DescriptorTable.NumDescriptorRanges = 1;

    D3D12_ROOT_SIGNATURE_DESC rsDesc {};
    rsDesc.NumParameters = 1;
    rsDesc.pParameters = rp;

    D3D12_STATIC_SAMPLER_DESC sampler = CD3DX12_STATIC_SAMPLER_DESC(0);
    sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    rsDesc.pStaticSamplers = &sampler;
    rsDesc.NumStaticSamplers = 1;
    rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ComPtr<ID3DBlob> rsBlob;
    ComPtr<ID3DBlob> errBlob;

    auto result = D3D12SerializeRootSignature(&rsDesc, D3D_ROOT_SIGNATURE_VERSION_1, rsBlob.ReleaseAndGetAddressOf(), errBlob.ReleaseAndGetAddressOf());
    assert(SUCCEEDED(result));

    result = device_->CreateRootSignature(0, rsBlob->GetBufferPointer(), rsBlob->GetBufferSize(), IID_PPV_ARGS(screenRSig_.ReleaseAndGetAddressOf()));
    assert(SUCCEEDED(result));

    std::unique_ptr<Shader> shader = std::make_unique<Shader>();
    shader->Create(L"Screen");

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
    gpsDesc.VS = {shader->GetVertexShader()->GetBufferPointer(), shader->GetVertexShader()->GetBufferSize()};
    gpsDesc.DepthStencilState.DepthEnable = false;
    gpsDesc.DepthStencilState.StencilEnable = false;

    gpsDesc.InputLayout.NumElements = 0;
    gpsDesc.InputLayout.pInputElementDescs = nullptr;
    gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    gpsDesc.NumRenderTargets = 4;
    gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    gpsDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    gpsDesc.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    gpsDesc.RTVFormats[3] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

    gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    gpsDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    gpsDesc.SampleDesc.Count = 1;
    gpsDesc.SampleDesc.Quality = 0;
    gpsDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
    gpsDesc.pRootSignature = screenRSig_.Get();

    assert(SUCCEEDED(result));

    gpsDesc.PS = {shader->GetPixelShader()->GetBufferPointer(), shader->GetPixelShader()->GetBufferSize()};
    result = device_->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(screenPipeline_.ReleaseAndGetAddressOf()));
    assert(SUCCEEDED(result));

    shader->PSLoad(L"Blur");
    gpsDesc.PS = {shader->GetPixelShader()->GetBufferPointer(), shader->GetPixelShader()->GetBufferSize()};
    result = device_->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(blurPipeline_.ReleaseAndGetAddressOf()));
    assert(SUCCEEDED(result));

    D3D12_GRAPHICS_PIPELINE_STATE_DESC blurResultDesc = {};
    blurResultDesc.VS = {shader->GetVertexShader()->GetBufferPointer(), shader->GetVertexShader()->GetBufferSize()};
    blurResultDesc.DepthStencilState.DepthEnable = false;
    blurResultDesc.DepthStencilState.StencilEnable = false;
    blurResultDesc.InputLayout.NumElements = 0;
    blurResultDesc.InputLayout.pInputElementDescs = nullptr;
    blurResultDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    blurResultDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    blurResultDesc.NumRenderTargets = 4;
    blurResultDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    blurResultDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    blurResultDesc.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    blurResultDesc.RTVFormats[3] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    blurResultDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    blurResultDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    blurResultDesc.SampleDesc.Count = 1;
    blurResultDesc.SampleDesc.Quality = 0;
    blurResultDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
    blurResultDesc.pRootSignature = screenRSig_.Get();

    shader->PSLoad(L"Bloom");
    blurResultDesc.PS = {shader->GetPixelShader()->GetBufferPointer(), shader->GetPixelShader()->GetBufferSize()};
    
	result = device_->CreateGraphicsPipelineState(&blurResultDesc, IID_PPV_ARGS(bloomPipeline_.ReleaseAndGetAddressOf()));
    assert(SUCCEEDED(result));
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
	const std::chrono::microseconds MIN_TIME(static_cast<uint64_t>(1e6 / maxFPS));
    const std::chrono::microseconds MIN_CHECK_TIME(static_cast<uint64_t>(1e6 / (maxFPS + 5)));

    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::microseconds elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

    if (elapsedTime < MIN_CHECK_TIME) {
	    while (std::chrono::steady_clock::now() - reference_ < MIN_TIME) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
	    }
    }

    reference_ = std::chrono::steady_clock::now();
}

#include <Psapi.h>
void DirectXCommon::DisplayInfo() {
    static ULONGLONG lastTime = 0, lastSysCPU = 0, lastUserCPU = 0;
    static int processors = 0;
    static HANDLE self = GetCurrentProcess();

    if (processors == 0){
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        processors = sysInfo.dwNumberOfProcessors;
        FILETIME ftime, fsys, fuser;
	    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	    lastSysCPU = (static_cast<ULONGLONG>(fsys.dwHighDateTime) << 32) | fsys.dwLowDateTime;
	    lastUserCPU = (static_cast<ULONGLONG>(fuser.dwHighDateTime) << 32) | fuser.dwLowDateTime;
	    lastTime = GetTickCount64();
    }

    FILETIME ftime, fsys, fuser;
    ULONGLONG now, sysCpu, userCpu;

    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    sysCpu = static_cast<ULONGLONG>(fsys.dwLowDateTime) | (static_cast<ULONGLONG>(fsys.dwHighDateTime) << 32);
    userCpu = static_cast<ULONGLONG>(fuser.dwLowDateTime) | (static_cast<ULONGLONG>(fuser.dwHighDateTime) << 32);
    now = GetTickCount64();

    float percent = ((sysCpu - lastSysCPU) + (userCpu - lastUserCPU)) / static_cast<float>(now - lastTime);
    lastSysCPU = sysCpu;
    lastUserCPU = userCpu;
    lastTime = now;

    //Memory
    size_t memory = 0;
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc))){
        memory = pmc.PrivateUsage / (1024 * 1024);
    }

    ImGuiManager::GetInstance()->AddCommand(this, [&]{
        ImGui::Begin("Info");
        ImGui::Text("FPS : %.2f", 1.0 / ImGui::GetIO().DeltaTime);
        ImGui::Text("Max FPS : %.2f", maxFPS);
        ImGui::Text("CPU : %.2f", (percent / processors) * 100.f);
        ImGui::Text("Memory : %d MB", memory);
        ImGui::End();
    });
}

void DirectXCommon::PreDraw() {
    //UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();

    barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier_.Transition.pResource = canvasResource_.Get();
    barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    commandList_->ResourceBarrier(1, &barrier_);

    barrier_.Transition.pResource = rootColorResource_.Get();
    barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    commandList_->ResourceBarrier(1, &barrier_);



    /*barrier_.Transition.pResource = swapChainBuffers_[backBufferIndex].Get();
    barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;*/


    D3D12_CPU_DESCRIPTOR_HANDLE* handles = new D3D12_CPU_DESCRIPTOR_HANDLE[SRV_INDEX_COUNT];
    auto rtvPointer = rtvForPP_->GetCPUDescriptorHandleForHeapStart();
    for (uint16_t i = 0; i < SRV_INDEX_COUNT; ++i){
        handles[i] = rtvPointer;
        rtvPointer.ptr += device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap_->GetCPUHandle(0);

    commandList_->OMSetRenderTargets(SRV_INDEX_COUNT, handles, false, &dsvHandle);
    float black[4] = {0, 0, 0, 1};
    commandList_->ClearRenderTargetView(handles[1], black, 0, nullptr);

    commandList_->ClearRenderTargetView(rtvForPP_->GetCPUDescriptorHandleForHeapStart(), &backColor_.x, 0, nullptr);
    commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);

    commandList_->RSSetViewports(1, &viewport_);
    commandList_->RSSetScissorRects(1, &scissorRect_);
}

void DirectXCommon::PostDraw() {
    //DisplayInfo();
	ImGuiManager::GetInstance()->AddCommand(this, [&]{
        ImGui::Begin("Screen");
        ImVec2 size = ImGui::GetContentRegionAvail();
        float aspect = static_cast<float>(WinApp::CLIENT_WIDTH) / static_cast<float>(WinApp::CLIENT_HEIGHT);
        size.y = size.x / aspect;
		ImGui::Image(ImTextureID(srvManager_->GetGPUHandle(indexes_[0]).ptr), size);
        ImGui::End();
    });
    SwitchToSwapChain();
}

void DirectXCommon::SwitchToSwapChain()  {
    D3D12_CPU_DESCRIPTOR_HANDLE* handles = new D3D12_CPU_DESCRIPTOR_HANDLE[SRV_INDEX_COUNT];
    auto rtvPointer = rtvForPP_->GetCPUDescriptorHandleForHeapStart();
    for (uint16_t i = 0; i < SRV_INDEX_COUNT; ++i){
        handles[i] = rtvPointer;
        rtvPointer.ptr += device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap_->GetCPUHandle(0);

    ///Blur
    commandList_->SetPipelineState(blurPipeline_.Get());
    commandList_->SetGraphicsRootSignature(screenRSig_.Get());
    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList_->IASetVertexBuffers(0, 1, &screenVBView_);

    barrier_.Transition.pResource = rootColorResource_.Get();
    barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    commandList_->ResourceBarrier(1, &barrier_);

    barrier_.Transition.pResource = blurResource_.Get();
    barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    commandList_->ResourceBarrier(1, &barrier_);

    commandList_->OMSetRenderTargets(SRV_INDEX_COUNT, handles, false, &dsvHandle);
    float black[4] = {0, 0, 0, 1};
    commandList_->ClearRenderTargetView(handles[2], black, 0, nullptr);

    srvManager_->PreDraw();
    auto handle = srvManager_->GetGPUHandle(indexes_[0]);
	commandList_->SetGraphicsRootDescriptorTable(0, handle);


    auto desc = rootColorResource_->GetDesc();
    D3D12_VIEWPORT vp = {};
    D3D12_RECT sc = {};

    vp.MaxDepth = 1.f;
    vp.MinDepth = 0.f;
    vp.Height = static_cast<float>(desc.Height) /2.f;
    vp.Width = static_cast<float>(desc.Width) / 2.f;
    sc.top = 0;
    sc.left = 0;
    sc.right = static_cast<LONG>(vp.Width);
    sc.bottom = static_cast<LONG>(vp.Height);

    for (int i = 0; i < 8; ++i){
        commandList_->RSSetViewports(1, &vp);
        commandList_->RSSetScissorRects(1, &sc);
        commandList_->DrawInstanced(3, 1, 0, 0);

        sc.top += static_cast<LONG>(vp.Height);
        vp.TopLeftX = 0;
        vp.TopLeftY = static_cast<float>(sc.top);

        vp.Width /= 2.f;
        vp.Height /= 2.f;
        sc.bottom = sc.top + static_cast<LONG>(vp.Height);
    }


    barrier_.Transition.pResource = blurResource_.Get();
    barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    commandList_->ResourceBarrier(1, &barrier_);

    ///Bloom
	barrier_.Transition.pResource = bloomResource_.Get();
    barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    commandList_->ResourceBarrier(1, &barrier_);

    commandList_->RSSetViewports(1, &viewport_);
    commandList_->RSSetScissorRects(1, &scissorRect_);

	commandList_->SetPipelineState(bloomPipeline_.Get());
    commandList_->SetGraphicsRootSignature(screenRSig_.Get());
    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList_->IASetVertexBuffers(0, 1, &screenVBView_);
    commandList_->OMSetRenderTargets(SRV_INDEX_COUNT, handles, false, nullptr);

    commandList_->ClearRenderTargetView(handles[3], black, 0, nullptr);

	srvManager_->PreDraw();
    handle = srvManager_->GetGPUHandle(indexes_[0]);
	commandList_->SetGraphicsRootDescriptorTable(0, handle);

	commandList_->DrawInstanced(3, 1, 0, 0);
    barrier_.Transition.pResource = bloomResource_.Get();
    barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    commandList_->ResourceBarrier(1, &barrier_);

    barrier_.Transition.pResource = canvasResource_.Get();
    barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    commandList_->ResourceBarrier(1, &barrier_);

    barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier_.Transition.pResource = swapChainBuffers_[swapChain_->GetCurrentBackBufferIndex()].Get();
    barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    commandList_->ResourceBarrier(1, &barrier_);

    auto bbIndex = swapChain_->GetCurrentBackBufferIndex();

    rtvPointer = rtvHandles_[bbIndex];

    commandList_->OMSetRenderTargets(1, &rtvPointer, false, nullptr);
    commandList_->ClearRenderTargetView(rtvPointer, &backColor_.x, 0, nullptr);

    commandList_->RSSetViewports(1, &viewport_);
    commandList_->RSSetScissorRects(1, &scissorRect_);

    commandList_->SetPipelineState(screenPipeline_.Get());

    srvManager_->PreDraw();
    handle = srvManager_->GetGPUHandle(indexes_[0]);

    commandList_->SetGraphicsRootDescriptorTable(0, handle);
    //commandList_->DrawInstanced(3, 1, 0, 0);
}

void DirectXCommon::EndFrame() {
    HRESULT hr = S_OK;

    auto bbi = swapChain_->GetCurrentBackBufferIndex();

    barrier_.Transition.pResource = swapChainBuffers_[bbi].Get();
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