#pragma once
#include <array>
#include <string>
#include <cassert>
#include <memory>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class WinApp;
class GraphicsPipeline;
class Shader;
class Heap;

using namespace Microsoft::WRL;

class DirectXCommon{

private://Variables
	//WinApp借り物
	WinApp* winApp_ = nullptr;

	//Factory
    ComPtr<IDXGIFactory7> factory_ = nullptr;
	//Device
    ComPtr<ID3D12Device> device_ = nullptr;

	//DebugLayer
    ComPtr<ID3D12Debug1> debugController_;

	//Command
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;
	ComPtr<ID3D12CommandQueue> commandQueue_;

	//SwapChain
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	std::array<ComPtr<ID3D12Resource>, 2> swapChainBuffers_;

	//RenderTargetView
	std::shared_ptr<Heap> rtvHeap_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2] {};

	//DSV
	ComPtr<ID3D12Resource> depthStencilResource_;
	std::shared_ptr<Heap> dsvHeap_;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_ {};

	//Barrier
	D3D12_RESOURCE_BARRIER barrier_ {};

	//Fence
	ComPtr<ID3D12Fence> fence_;
	uint32_t fenceValue_ = 0;

	//Mainにおいている。場所は要検討。
	//std::shared_ptr<D3DResourceLeakChecker> leakChecker_;

	//Shader
    std::shared_ptr<Shader> shader_;

	//GraphicsPipeline
	std::shared_ptr<GraphicsPipeline> graphicsPipeline_;

	//Viewport
	D3D12_VIEWPORT viewport_ {};

	//scissor
	D3D12_RECT scissorRect_ {};

public://Methods
	~DirectXCommon();
	bool Initialize(WinApp* winApp);

	void PreDraw();
	void PostDraw();

	static ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
    static ID3D12Resource* CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height);

	ID3D12DescriptorHeap* CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT count, bool shaderVisible) const;

	ID3D12Device* GetDevice() const {
		return device_.Get();
	}
	ID3D12GraphicsCommandList* GetCommandList() const {
		return commandList_.Get();
	}
    Shader* GetShader() const {
        return shader_.get();
    }
	size_t GetBackBufferCount() const {
		return swapChainBuffers_.size();
	}

private://Methods

	void CreateDebugLayer();
	void CreateFactory();
	void CreateDevice();
	void CreateCommand();
	void CreateSwapChain(HWND hwnd, int width, int height);
	void CreateFence();
	void CompileShader();
	void CreateGraphicsPipeline();
	void SettingGraphicsInfo();
	//void CreateShaderResourceView();
	void CreateDepthStencilView();
	
	void WaitForCommandQueue();
	void EndFrame();
};

