#pragma once
#include <cstdint>
#include <d3d12.h>
#include <wrl/client.h>

class Heap{
    //借りポ
    ID3D12Device* device_ = nullptr;

    //Heap
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;
    D3D12_DESCRIPTOR_HEAP_TYPE type_ {};

    //
    uint32_t useCount_ = 0;

public:
	bool Create(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t useCount, bool shaderVisible);

    ID3D12DescriptorHeap* GetHeap() const {
        return heap_.Get();
    }

    D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32_t index) const;
    D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t index) const;
};

