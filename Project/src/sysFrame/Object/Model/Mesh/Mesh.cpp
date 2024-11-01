#include "Mesh.h"

#include <fstream>
#include <sstream>

#include "DirectX/DirectXCommon.h"
#include "DirectX/ObjectCommon/MeshCommon.h"
#include "DirectX/Texture/TextureManager.h"
#include "System/Math/Vector3.h"


Mesh::MaterialData Mesh::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& fileName) {
    MaterialData materialData {};
    std::string line;
    std::ifstream file(directoryPath + "/" + fileName);
    assert(file.is_open());
    while (std::getline(file, line)){
        std::string identifier;
        std::istringstream s(line);
        s >> identifier;

        if (identifier == "map_Kd"){
            std::string textureFileName;
            s >> textureFileName;

            materialData.texturePath = directoryPath + "/" + textureFileName;
        }
    }
    return materialData;
}

Mesh::ModelData Mesh::LoadObjFile(const std::string& directoryPath, const std::string& fileName) {
    ModelData modelData {};
    std::vector<Vector4> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> texcoords;
    std::string line;

    std::ifstream file(directoryPath + "/" + fileName);
    assert(file.is_open());

    while (std::getline(file, line)){
        std::string identifier;
        std::istringstream s(line);
        s >> identifier;

        if (identifier == "v"){
            Vector4 position {};
            s >> position.x >> position.y >> position.z;
            position.w = 1;
            positions.push_back(position);
        } else if (identifier == "vt"){
            Vector2 texcoord {};
            s >> texcoord.x >> texcoord.y;
            texcoords.push_back(texcoord);
        } else if (identifier == "vn"){
            Vector3 normal {};
            s >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (identifier == "f"){
            VertexData triangle[3];
            for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex){
                std::string vertexDefinition;
                s >> vertexDefinition;

                std::istringstream v(vertexDefinition);
                uint32_t elementIndices[3];

                for (int32_t element = 0; element < 3; ++element){
                    std::string index;
                    std::getline(v, index, '/');
                    elementIndices[element] = std::stoi(index);
                }

                Vector4 position = positions[elementIndices[0] - 1];
                Vector2 texcoord = texcoords[elementIndices[1] - 1];
                Vector3 normal = normals[elementIndices[2] - 1];

                position.x *= -1;
                texcoord.y = 1 - texcoord.y;
                normal.x *= -1;

                triangle[faceVertex] = {position, texcoord, /*normal*/};
            }
            modelData.vertices.push_back(triangle[2]);
            modelData.vertices.push_back(triangle[1]);
            modelData.vertices.push_back(triangle[0]);
        } else if (identifier == "mtllib"){
            std::string materialFileName;
            s >> materialFileName;

            modelData.material = LoadMaterialTemplateFile(directoryPath, materialFileName);
        }
    }

    return modelData;
}

void Mesh::Initialize(const std::string& directory, const std::string& name) {
    dxCommon_ = common_->GetDXCommon();
    commandList_ = dxCommon_->GetCommandList();

	modelData_ = LoadObjFile(directory, name);

    vertexResource_.Attach(DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * modelData_.vertices.size()));

    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = static_cast<UINT>(sizeof(VertexData) * modelData_.vertices.size());
    vertexBufferView_.StrideInBytes = sizeof(VertexData);

    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
    memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

    materialResource_.Attach(DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material)));
    materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&material_));

    material_->color = {1, 1, 1, 1};


    TextureManager::GetInstance()->Load(modelData_.material.texturePath);
}

void Mesh::Draw() {
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
    commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
    commandList_->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetGPUHandle(modelData_.material.texturePath));

    commandList_->DrawInstanced(static_cast<UINT>(modelData_.vertices.size()), 1, 0, 0);
}
