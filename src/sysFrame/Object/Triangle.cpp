#include "Triangle.h"

#include "System/Math/VertexData.h"

void Triangle::Initialize() {
    vertexResource_.Attach(DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 3));
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = sizeof(VertexData) * 3;
    vertexBufferView_.StrideInBytes = sizeof(VertexData);

    VertexData* vertexData = nullptr;

    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

    vertexData[0].position = {-.5f, -.5f, 0, 1};
    vertexData[1].position = {0, .5f, 0, 1};
    vertexData[2].position = {.5f, -.5f, 0, 1};
}

void Triangle::Update() {
}

void Triangle::Draw() {
    commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList_->DrawInstanced(3, 1, 0, 0);
}
