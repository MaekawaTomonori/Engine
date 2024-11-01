#include "Sprite.h"

#include <memory>

#include "imgui/imgui.h"

#include "WindowsApplication/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Texture/TextureManager.h"
#include "System/Math/Material.h"
#include "System/Math/MathUtils.h"

void Sprite::AdjustTextureSize() {
    const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetTextureMetadata(texturePath);
    texSize = {static_cast<float>(metadata.width), static_cast<float>(metadata.height)};
    size = texSize;
}

void Sprite::Initialize() {
    //VertexData
	vertexResource_.Attach(DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 4));
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
    vertexBufferView_.StrideInBytes = sizeof(VertexData);

    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

    vertexData_[0].position = {0, 1.f, 0, 1};
    vertexData_[1].position = {0, 0, 0, 1};
    vertexData_[2].position = {1.f, 1.f, 0, 1};
    vertexData_[3].position = {1.f, 0, 0, 1};

    vertexData_[0].texcoord = {0, 1};
    vertexData_[1].texcoord = {0, 0};
    vertexData_[2].texcoord = {1, 1};
    vertexData_[3].texcoord = {1, 0};

    //IndexData
    indexResource_.Attach(DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(uint32_t) * 6));

    indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
    indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
    indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

    indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

    indexData_[0] = 0;
    indexData_[1] = 1;
    indexData_[2] = 2;
    indexData_[3] = 1;
    indexData_[4] = 3;
    indexData_[5] = 2;

    //MaterialData
    materialResource_.Attach(DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material)));
    materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&material_));

    material_->color = {1, 1, 1, 1};

    worldTransform_ = std::make_shared<WorldTransform>(dxCommon_);
    worldTransform_->Initialize();
    worldTransform_->scale = {1,1,1};
    worldTransform_->rotate = {0, 0, 0};
    worldTransform_->translate = {0, 0, 0};

    size = {100, 100};

    AdjustTextureSize();
}

void Sprite::Update() {
    ImGui::Begin("Sprite");
    if (ImGui::TreeNode(uuid_.c_str())){
        ImGui::DragFloat2("Pos : ", &position.x, 0.1f);
        ImGui::DragFloat2("Scale : ", &size.x, 0.1f);
        ImGui::DragFloat("Rotation : ", &rotation, 0.01f);
        ImGui::ColorEdit4("Color", &material_->color.x);

        ImGui::TreePop();
    }
    ImGui::End();

#pragma region Vertex position
    worldTransform_->translate= {position.x, position.y, 0};
    worldTransform_->rotate = {0, 0, rotation};
    worldTransform_->scale = {size.x, size.y, 1};

    float left = 0.f - anchorPoint.x;
    float right = 1.f - anchorPoint.x;
    float top = 0 - anchorPoint.y;
    float bottom = 1.f - anchorPoint.y;

    if(flipX){
        left = -left;
        right = -right;
    }

    if (flipY){
        top = -top;
        bottom = -bottom;
    }

    vertexData_[0].position = {left, bottom, 0, 1};
    vertexData_[1].position = {left, top, 0, 1};
    vertexData_[2].position = {right, bottom, 0, 1};
    vertexData_[3].position = {right, top, 0, 1};

#pragma endregion

#pragma region Vertex texcoord
    const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetTextureMetadata(texturePath);
    float texLeft = leftTop.x / metadata.width;
    float texRight = (leftTop.x + texSize.x) / metadata.width;
    float texTop = leftTop.y / metadata.height;
    float texBottom = (leftTop.y + texSize.y) / metadata.height;

    vertexData_[0].texcoord = {texLeft, texBottom};
    vertexData_[1].texcoord = {texLeft, texTop};
    vertexData_[2].texcoord = {texRight, texBottom};
    vertexData_[3].texcoord = {texRight, texTop};
#pragma endregion

    Matrix4x4 worldM = MathUtils::Matrix::MakeAffineMatrix(worldTransform_->scale, worldTransform_->rotate, worldTransform_->translate);
    Matrix4x4 viewProjection = MathUtils::Matrix::MakeIdentity() * MathUtils::Matrix::MakeOrthogonalMatrix(0, WinApp::CLIENT_WIDTH, 0, WinApp::CLIENT_HEIGHT, 0, 100.f);

    worldTransform_->SetWVP(worldM * viewProjection);
}

void Sprite::Draw() {
    commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
    commandList_->IASetIndexBuffer(&indexBufferView_);
    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
    commandList_->SetGraphicsRootConstantBufferView(1, worldTransform_->GetGPUVirtualAddress());
    commandList_->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetGPUHandle(texturePath));

    commandList_->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
