#include "Heap.h"

#include <cassert>

bool Heap::Create(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t useCount, bool shaderVisible) {
    device_ = device;
    type_ = type;
    useCount_ = useCount;

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = type_;
    desc.NumDescriptors = useCount_;
    desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    HRESULT hr = device_->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap_));
    assert(SUCCEEDED(hr));

    return true;
}

D3D12_CPU_DESCRIPTOR_HANDLE Heap::GetCPUHandle(uint32_t index) const {
    D3D12_CPU_DESCRIPTOR_HANDLE handle = heap_->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += device_->GetDescriptorHandleIncrementSize(type_) * index;
    return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE Heap::GetGPUHandle(uint32_t index) const {
    D3D12_GPU_DESCRIPTOR_HANDLE handle = heap_->GetGPUDescriptorHandleForHeapStart();
    handle.ptr += device_->GetDescriptorHandleIncrementSize(type_) * index;
    return handle;
}
