#pragma once
#include <d3d12.h>
#include <dxcapi.h>
#include <memory>
#include <wrl/client.h>

class Heap;
class DirectXCommon;

class GraphicsPipeline{
public:
	void Create(ID3D12Device* device, IDxcBlob* vs, IDxcBlob* ps);
	void Create2D(ID3D12Device* device);

	void DrawCall(ID3D12GraphicsCommandList* commandList) const;

private://Methods
	void CreateRootSignature();
	void DescriptorRange();
	void CreateInputLayout();
	void CreateBlendState();
	void CreateRasterizerState();
	void CreateSampler();
	void CreateDepthStencil();

	void CreatePSO();

private://Variables
	//借りポ
	ID3D12Device* device_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	D3D12_ROOT_PARAMETER rootParamerters_[4] = {};
    D3D12_DESCRIPTOR_RANGE descriptorRange_[1] {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_ {};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3]{};
	D3D12_BLEND_DESC blendDesc_ {};
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_;
	D3D12_RASTERIZER_DESC rasterizerDesc_ {};
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_;

	D3D12_STATIC_SAMPLER_DESC staticSamplers_[1] = {};

    //DepthStencil
    D3D12_DEPTH_STENCIL_DESC depthStencilDesc_ {};

	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;
};
