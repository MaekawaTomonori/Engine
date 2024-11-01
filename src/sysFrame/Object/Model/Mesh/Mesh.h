#pragma once
#include <d3d12.h>
#include <string>
#include <vector>
#include <wrl/client.h>

#include "System/Math/Material.h"
#include "System/Math/VertexData.h"

class DirectXCommon;
class MeshCommon;

class Mesh{
	struct MaterialData{
		std::string texturePath;
	};

	struct ModelData{
		std::vector<VertexData> vertices;
        MaterialData material;
	};
private:
	static MaterialData LoadMaterialTemplateFile(const std::string& path, const std::string& fileName);
    static ModelData LoadObjFile(const std::string& path, const std::string& fileName);

private:
	//MeshCommon ptr (借りポ)
	MeshCommon* common_ = nullptr;

    //借りポ from meshCommon
    DirectXCommon* dxCommon_ = nullptr;
    ID3D12GraphicsCommandList* commandList_ = nullptr;

	//ModelData from .obj file
    ModelData modelData_;


	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
    Material* material_ = nullptr;

    VertexData* vertexData_ = nullptr;

public:
	Mesh(MeshCommon* meshCommon) :common_(meshCommon) {}

	void Initialize(const std::string& directory, const std::string& name);
	void Draw();
};

