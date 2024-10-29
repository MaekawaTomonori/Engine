#include "SRVManager.h"

#include <memory>

#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/Heap.h"

const uint32_t SRVManager::kMaxSRVCount = 512;

void SRVManager::Initialize(DirectXCommon* dxCommon) {
    dxCommon_ = dxCommon;

    heap_ = std::make_shared<Heap>();
    heap_->Create(dxCommon_->GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount, true);
    descriptorSize = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    useIndex_ = 0;
}

uint32_t SRVManager::Allocate() {
    assert(useIndex_ <= kMaxSRVCount);

    uint32_t index = useIndex_;

    ++useIndex_;

    return index;
}

void SRVManager::PreDraw() const {
    ID3D12DescriptorHeap* descriptorHeaps[] = {heap_->GetDescriptorHeap()};
    dxCommon_->GetCommandList()->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
}

void SRVManager::CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipMap) {
    D3D12_SHADER_RESOURCE_VIEW_DESC desc {};
    desc.Format = format;
    desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipLevels = mipMap;

    dxCommon_->GetDevice()->CreateShaderResourceView(pResource, &desc, heap_->GetCPUHandle(srvIndex));
}

void SRVManager::CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements,
	UINT stride) {
    (void)srvIndex, pResource, numElements, stride;
}

void SRVManager::SetGraphicsRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex) const {
    dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(rootParameterIndex, heap_->GetGPUHandle(srvIndex));
}

//ID3D12DescriptorHeap* SRVManager::GetDescriptorHeap() const {
//	return heap_->GetDescriptorHeap();
//}

//D3D12_GPU_DESCRIPTOR_HANDLE SRVManager::GetGPUHandle(uint32_t index) const {
//    assert(index <= useIndex_);
//    return heap_->GetGPUHandle(index);
//}
