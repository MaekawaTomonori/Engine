#pragma once
#include <d3d12.h>
#include <dxcapi.h>
#include <wrl/client.h>

class DirectXCommon;

class GraphicsPipeline{
public:
	void Create(ID3D12Device* device, IDxcBlob* vs, IDxcBlob* ps);

	void DrawCall(ID3D12GraphicsCommandList* commandList) const;

private://Methods
	void CreateRootSignature(ID3D12Device* device);
	void DescriptorRange();
	void CreateInputLayout();
	void CreateBlendState();
	void CreateRasterizerState();
	void CreateSampler();

	void CreatePSO(ID3D12Device* device);

private://Variables
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	D3D12_ROOT_PARAMETER rootParamerters_[3] = {};
    D3D12_DESCRIPTOR_RANGE descriptorRange_[1] {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_ {};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[2]{};
	D3D12_BLEND_DESC blendDesc_ {};
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_;
	D3D12_RASTERIZER_DESC rasterizerDesc_ {};
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_;

	D3D12_STATIC_SAMPLER_DESC staticSamplers_[1] = {};

	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;
};
