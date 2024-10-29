#include "Sprite.h"

#include <memory>

#include "Application/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Texture/TextureManager.h"
#include "imgui/imgui.h"
#include "System/Math/Material.h"
#include "System/Math/MathUtils.h"

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
    worldTransform_->transform_ = {
        {1,1,1},
        {0,0,0},
        {0,0,0}
    };
}

void Sprite::Update() {
    ImGui::Begin("Sprite");
    ImGui::DragFloat3("Pos : ", &position.x, 0.01f);
    ImGui::DragFloat3("Scale : ", &size.x, 0.01f);
    ImGui::DragFloat("Rotation : ", &rotation, 0.01f);
    ImGui::ColorEdit4("Color", &material_->color.x);
    ImGui::End();

    worldTransform_->transform_ .translate= {position.x, position.y, 0};
    worldTransform_->transform_.rotate = {0, 0, rotation};
    worldTransform_->transform_.scale = {size.x, size.y, 1};

    Matrix4x4 worldM = MathUtils::Matrix::MakeAffineMatrix(worldTransform_->transform_);
    Matrix4x4 viewProjection = MathUtils::Matrix::MakeIdentity() * MathUtils::Matrix::MakeOrthogonalMatrix(0, WinApp::CLIENT_WIDTH, 0, WinApp::CLIENT_HEIGHT, 0, 100.f);

    worldTransform_->SetWVP(worldM * viewProjection);
}

void Sprite::Draw() {
    commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
    commandList_->IASetIndexBuffer(&indexBufferView_);
    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
    commandList_->SetGraphicsRootConstantBufferView(1, worldTransform_->GetGPUVirtualAddress());
    commandList_->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetGPUHandle(textureIndex));

    commandList_->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
