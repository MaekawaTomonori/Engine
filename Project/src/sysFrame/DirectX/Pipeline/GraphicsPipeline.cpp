#include "GraphicsPipeline.h"

#include <cassert>
#include <format>

#include "WindowsApplication/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/Heap.h"
#include "System/System.h"

void GraphicsPipeline::Create(ID3D12Device* device, IDxcBlob* vs, IDxcBlob* ps) {
    device_ = device;

    CreateRootSignature();
    CreateInputLayout();
    CreateBlendState();
    CreateRasterizerState();
    vertexShaderBlob_.Attach(vs);
    pixelShaderBlob_.Attach(ps);

    CreatePSO();
}

void GraphicsPipeline::Create2D(ID3D12Device* device) {
    device_ = device;

    CreateRootSignature();

    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc {};
    desc.pRootSignature = rootSignature_.Get();
    HRESULT hr = device_->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&graphicsPipelineState_));
    assert(SUCCEEDED(hr));
}

void GraphicsPipeline::DrawCall(ID3D12GraphicsCommandList* commandList) const {
    commandList->SetGraphicsRootSignature(rootSignature_.Get());
    commandList->SetPipelineState(graphicsPipelineState_.Get());
}

void GraphicsPipeline::DescriptorRange() {
    descriptorRange_[0].BaseShaderRegister = 0;
    descriptorRange_[0].NumDescriptors = 1;
    descriptorRange_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriptorRange_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}

void GraphicsPipeline::CreateRootSignature() {
    HRESULT hr = S_OK;

    D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature {};
    descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    //PixelShader
    rootParamerters_[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParamerters_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParamerters_[0].Descriptor.ShaderRegister = 0;

    //VertexShader
    rootParamerters_[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParamerters_[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    rootParamerters_[1].Descriptor.ShaderRegister = 0;

    DescriptorRange();

    //DescriptorTable
    rootParamerters_[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParamerters_[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParamerters_[2].DescriptorTable.pDescriptorRanges = descriptorRange_;
    rootParamerters_[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_);

    //Lighting
    rootParamerters_[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParamerters_[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParamerters_[3].Descriptor.ShaderRegister = 1;

	//set
    descriptionRootSignature.pParameters = rootParamerters_;
    descriptionRootSignature.NumParameters = _countof(rootParamerters_);

    //StaticSampler
    CreateSampler();
    descriptionRootSignature.pStaticSamplers = staticSamplers_;
    descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers_);

    //Serialize
    ComPtr<ID3DBlob> signatureBlob;
    ComPtr<ID3DBlob> errorBlob;

    hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

    if(FAILED(hr)){
        System::Log(static_cast<char*>(errorBlob->GetBufferPointer()));
        assert(false);
    }

    hr = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
    assert(SUCCEEDED(hr));
}

void GraphicsPipeline::CreateInputLayout() {
    inputElementDescs_[0].SemanticName = "POSITION";
    inputElementDescs_[0].SemanticIndex = 0;
    inputElementDescs_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    inputElementDescs_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
    inputElementDescs_[0].InputSlot = 0;

    inputElementDescs_[1].SemanticName = "TEXCOORD";
    inputElementDescs_[1].SemanticIndex = 0;
    inputElementDescs_[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputElementDescs_[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

    inputElementDescs_[2].SemanticName = "NORMAL";
    inputElementDescs_[2].SemanticIndex = 0;
    inputElementDescs_[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputElementDescs_[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

    //System::Debug::Log(std::format(L"InputElementSlot : {}\n", inputElementDescs_[0].InputSlot));

	inputLayoutDesc_.pInputElementDescs = inputElementDescs_;
    inputLayoutDesc_.NumElements = _countof(inputElementDescs_);
}

void GraphicsPipeline::CreateBlendState() {
    blendDesc_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
}

void GraphicsPipeline::CreateRasterizerState() {
    rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
    rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;
}

void GraphicsPipeline::CreateSampler() {
    staticSamplers_[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    staticSamplers_[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers_[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers_[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers_[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    staticSamplers_[0].MaxLOD = D3D12_FLOAT32_MAX;
    staticSamplers_[0].ShaderRegister = 0;
    staticSamplers_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
}

void GraphicsPipeline::CreateDepthStencil() {
    depthStencilDesc_.DepthEnable = true;
    depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}

void GraphicsPipeline::CreatePSO() {
    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc {};
    graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();
    graphicsPipelineStateDesc.InputLayout = inputLayoutDesc_;
    graphicsPipelineStateDesc.BlendState = blendDesc_;
    graphicsPipelineStateDesc.VS = {vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize()};
    graphicsPipelineStateDesc.RasterizerState = rasterizerDesc_;
    graphicsPipelineStateDesc.PS = {pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize()};
    graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc_;
    graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    graphicsPipelineStateDesc.NumRenderTargets = 1;
    graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    graphicsPipelineStateDesc.SampleDesc.Count = 1;
    graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

    HRESULT hr = device_->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState_));
    assert(SUCCEEDED(hr));
}
