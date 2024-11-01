#include "TextureManager.h"

#include <format>

#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/Heap.h"
#include "DirectX/Heap/SRVManager.h"
#include "DirectXTex/d3dx12.h"
#include "System/System.h"

std::shared_ptr<TextureManager> TextureManager::instance_ = nullptr;

void TextureManager::InstanceInit() {
    instance_ = std::shared_ptr<TextureManager>(new TextureManager, [](const TextureManager* ptr) {
        delete ptr;
    });
}

DirectX::ScratchImage TextureManager::LoadTexture(const std::string& filename) const {
    DirectX::ScratchImage image {};
    std::string fullpath = folderPath_ + filename;
    std::wstring filePathW = System::ConvertString(fullpath);
    HRESULT hr = LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
    assert(SUCCEEDED(hr));

    DirectX::ScratchImage mipImages {};
    hr = GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
    assert(SUCCEEDED(hr));

    return mipImages;
}

ID3D12Resource* TextureManager::CreateTextureResource(const DirectX::TexMetadata& metadata) {
    /// FLOW  ///
	/// 1. Resource setting from metadata
	/// 2. Heap setting
	/// 3. Generate Resource
	///

	//Step1
	//Setting Resource from Metadata
    D3D12_RESOURCE_DESC resourceDesc {};
    resourceDesc.Width = static_cast<UINT>(metadata.width);
    resourceDesc.Height = static_cast<UINT>(metadata.height);
    resourceDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
    resourceDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
    resourceDesc.Format = metadata.format;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);

    //Step2
    //HEAP SETTINGs
    D3D12_HEAP_PROPERTIES heapProperties {};
    heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
    //heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    //heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

    //Step3
    //Generate Resource
    ID3D12Resource* resource = nullptr;

    #ifdef _DEBUG
    HRESULT hr =
        #endif  
        dxCommon_->GetDevice()->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&resource)
        );
    assert(SUCCEEDED(hr));

    return resource;
}

[[nodiscard]]
ID3D12Resource* TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages) const {
    std::vector<D3D12_SUBRESOURCE_DATA> subResources;
    PrepareUpload(dxCommon_->GetDevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subResources);
    uint32_t intermediateSize = static_cast<uint32_t>(GetRequiredIntermediateSize(texture, 0, static_cast<UINT>(subResources.size())));
    ID3D12Resource* intermediateResource = DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), intermediateSize);
    UpdateSubresources(dxCommon_->GetCommandList(), texture, intermediateResource, 0, 0, static_cast<UINT>(subResources.size()), subResources.data());

    D3D12_RESOURCE_BARRIER barrier {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = texture;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
    dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);
    return intermediateResource;
}

std::shared_ptr<TextureManager> TextureManager::GetInstance() {
    if(!instance_){
        InstanceInit();
    }

    return instance_;
}

void TextureManager::Initialize(DirectXCommon* dxCommon, SRVManager* srvManager) {
    dxCommon_ = dxCommon;
    srvManager_ = srvManager;
}

void TextureManager::Load(const std::string& fileName) {
    std::string name;
    size_t pos = fileName.find_last_of('/');
    if (pos != std::string::npos){
    	name = fileName.substr(pos + 1);
    } else{
        name = fileName;
    }

	if (textures_.contains(name)){
        return;
    }

    assert(!srvManager_->IsFull());

    

    Texture& texture = textures_[name];

    DirectX::ScratchImage img = LoadTexture(name);

    texture.metadata = img.GetMetadata();
    texture.resource = CreateTextureResource(img.GetMetadata());
    texture.intermediateResource = UploadTextureData(texture.resource.Get(), img);

    texture.srvIndex = srvManager_->Allocate();
    texture.cpuHandle = srvManager_->GetCPUHandle(texture.srvIndex);
    texture.gpuHandle = srvManager_->GetGPUHandle(texture.srvIndex);

    srvManager_->CreateSRVforTexture2D(texture.srvIndex, texture.resource.Get(), texture.metadata.format, static_cast<UINT>(texture.metadata.mipLevels));
}

const DirectX::TexMetadata& TextureManager::GetTextureMetadata(const std::string& fileName) const {
    if (textures_.contains(fileName)){
        return textures_.at(fileName).metadata;
	}

    assert(0);
    return textures_.at("").metadata;
}

uint32_t TextureManager::GetSrvIndex(const std::string& fileName) const {
    if (textures_.contains(fileName)){
        return textures_.at(fileName).srvIndex;
    }

    assert(0);
    return 0;
}

uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& path) const {
    if (textures_.contains(path)){
        return textures_.at(path).srvIndex;
    }

    assert(0);
    return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUHandle(const std::string& fileName) const {
    if (textures_.contains(fileName)){
        return textures_.at(fileName).gpuHandle;
    }

    assert(0);
    return {};
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUHandle(const uint32_t index) const {
    assert(index <= textures_.size());
    System::Log(std::format("TextureManager::GetGPUHandle: index {}", index));
	return srvManager_->GetGPUHandle(index);
}
