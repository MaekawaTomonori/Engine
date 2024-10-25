#include "GraphicsPipeline.h"

#include <d3d12.h>
#include <format>

#include "DirectX/DirectXCommon.h"
#include "System/System.h"

void GraphicsPipeline::Create(ID3D12Device* device, IDxcBlob* vs, IDxcBlob* ps) {
    CreateRootSignature(device);
    CreateInputLayout();
    CreateBlendState();
    CreateRasterizerState();
    vertexShaderBlob_.Attach(vs);
    pixelShaderBlob_.Attach(ps);

    CreatePSO(device);
}

void GraphicsPipeline::DrawCall(ID3D12GraphicsCommandList* commandList) const {
    commandList->SetGraphicsRootSignature(rootSignature_.Get());
    commandList->SetPipelineState(graphicsPipelineState_.Get());
}

void GraphicsPipeline::CreateRootSignature(ID3D12Device* device) {
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

    //set
    descriptionRootSignature.pParameters = rootParamerters_;
    descriptionRootSignature.NumParameters = _countof(rootParamerters_);

    //Serialize
    ComPtr<ID3DBlob> signatureBlob;
    ComPtr<ID3DBlob> errorBlob;

    hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

    if(FAILED(hr)){
        System::Log(static_cast<char*>(errorBlob->GetBufferPointer()));
        assert(false);
    }

    hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
    assert(SUCCEEDED(hr));
}

void GraphicsPipeline::CreateInputLayout() {
    inputElementDescs_[0].SemanticName = "POSITION";
    inputElementDescs_[0].SemanticIndex = 0;
    inputElementDescs_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    inputElementDescs_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
    inputElementDescs_[0].InputSlot = 0;

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

void GraphicsPipeline::CreatePSO(ID3D12Device* device) {
    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc {};
    graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();
    graphicsPipelineStateDesc.InputLayout = inputLayoutDesc_;
    graphicsPipelineStateDesc.BlendState = blendDesc_;
    graphicsPipelineStateDesc.VS = {vertexShaderBlob_->GetBufferPointer(), vertexShaderBlob_->GetBufferSize()};
    graphicsPipelineStateDesc.RasterizerState = rasterizerDesc_;
    graphicsPipelineStateDesc.PS = {pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize()};

    graphicsPipelineStateDesc.NumRenderTargets = 1;
    graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    graphicsPipelineStateDesc.SampleDesc.Count = 1;
    graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

    HRESULT hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState_));
    assert(SUCCEEDED(hr));
}
