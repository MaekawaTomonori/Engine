#include "Triangle.h"

#include "System/Math/Material.h"
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
}

void Triangle::Update() {
}

void Triangle::Draw() {
    commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
    commandList_->DrawInstanced(3, 1, 0, 0);
}
