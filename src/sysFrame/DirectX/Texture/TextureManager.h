#pragma once
#include <d3d12.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <wrl/client.h>

#include "DirectXTex/DirectXTex.h"

class DirectXCommon;
class SRVManager;

class TextureManager{
	struct Texture{
        uint32_t srvIndex;
        DirectX::TexMetadata metadata;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
        D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	};

private: //Variables
	static std::shared_ptr<TextureManager> instance_;

    DirectXCommon* dxCommon_ = nullptr;
    SRVManager* srvManager_ = nullptr;

	std::string folderPath_ = "assets/Resources/";

    std::unordered_map<std::string, Texture> textures_;

private: //Methods
	TextureManager() = default;
    ~TextureManager() = default;
	static void InstanceInit();

   DirectX::ScratchImage LoadTexture(const std::string& filename);
   ID3D12Resource* CreateTextureResource(const DirectX::TexMetadata& metadata);
	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages) const;

public:
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
	static std::shared_ptr<TextureManager> GetInstance();

	void Initialize(DirectXCommon* dxCommon, SRVManager* srvManager);
	void Load(const std::string& fileName);

    const DirectX::TexMetadata& GetTextureMetadata(const std::string& fileName) const;
    uint32_t GetSrvIndex(const std::string& fileName) const;

	uint32_t GetTextureIndexByFilePath(const std::string& path) const;

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(const std::string& fileName) const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(const uint32_t index) const;
};

