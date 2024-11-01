#include "Triangle.h"

#include "DirectX/Texture/TextureManager.h"
#include "imgui/imgui.h"
#include "System/Math/Material.h"
#include "System/Math/MathUtils.h"
#include "System/Math/VertexData.h"

void Triangle::Initialize() {
    vertexResource_.Attach(DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 3));
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = sizeof(VertexData) * 3;
    vertexBufferView_.StrideInBytes = sizeof(VertexData);

    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

    vertexData_[0].position = {-.5f, -.5f, 0, 1};
    vertexData_[1].position = {0, .5f, 0, 1};
    vertexData_[2].position = {.5f, -.5f, 0, 1};

    materialResource_.Attach(DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material)));

    materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&material_));

    material_->color = {1, 0, 0, 1};

    worldTransform_ = std::make_shared<WorldTransform>(dxCommon_);
    worldTransform_->Initialize();
    worldTransform_->scale = {1,1,1};
    worldTransform_->rotate = {0,0,0};
    worldTransform_->translate = {0,0,0};

    texturePath = "uvChecker.png";
}

void Triangle::Update() {

    worldTransform_->rotate.y += 0.03f;

    ImGui::Begin("Triangle");
    ImGui::DragFloat3("Pos : ", &worldTransform_->translate.x, 0.01f);
    ImGui::End();

    worldTransform_->Update();
}

void Triangle::Draw() {
    commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
    commandList_->SetGraphicsRootConstantBufferView(1, worldTransform_->GetGPUVirtualAddress());
    commandList_->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetGPUHandle(texturePath));

    commandList_->DrawInstanced(3, 1, 0, 0);
}
