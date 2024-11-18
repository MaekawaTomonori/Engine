#pragma once
#include <d3d12.h>
#include <dxcapi.h>
#include <memory>
#include <wrl/client.h>

#include "DirectX/Shader/Shader.h"

class Heap;
class DirectXCommon;

enum class BlendMode{
	NONE,
	ALPHA,
	ADD,
	SUB,
	MULTI,
	SCREEN,
};

class GraphicsPipeline{
public:
	enum class Type{
		MODEL,
		SPRITE,
		PARTICLE,
	};

	void Create(DirectXCommon* dxCommon, Type type);

	void DrawCall(ID3D12GraphicsCommandList* commandList) const;

    void SetBlendMode(BlendMode mode) {
        blendMode_ = mode;
    }

private://Methods
	void CreateRootSignature();
	void DescriptorRange();
	void CreateInputLayout();
	void CreateBlendState();
	void CreateShader(const std::wstring& name);
	void CreateRasterizerState();
	void CreateSampler();
	void CreateDepthStencil();

	void CreatePSO();

private://Variables
	//借りポ
	DirectXCommon* dxCommon_ = nullptr;

	Type type_ = Type::MODEL;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	D3D12_ROOT_PARAMETER rootParamerters_[4] = {};
    D3D12_DESCRIPTOR_RANGE descriptorRange_[1] {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_ {};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3]{};
    BlendMode blendMode_ = BlendMode::NONE;
	D3D12_BLEND_DESC blendDesc_ {};
	D3D12_RASTERIZER_DESC rasterizerDesc_ {};

	D3D12_STATIC_SAMPLER_DESC staticSamplers_[1] = {};

	std::unique_ptr<Shader> shader_;

    //DepthStencil
    D3D12_DEPTH_STENCIL_DESC depthStencilDesc_ {};

	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;
};
