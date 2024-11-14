#include "Model.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/ObjectCommon/ModelCommon.h"
#include "imgui/imgui.h"
#include "Mesh/Mesh.h"

Model::~Model() = default;

void Model::Initialize() {
    dxCommon_ = modelCommon_->GetDXCommon();
    commandList_ = dxCommon_->GetCommandList();

    worldTransform_ = std::make_unique<WorldTransform>(dxCommon_);
    worldTransform_->Initialize();

}

void Model::Update() const {
#ifdef _DEBUG
    ImGui::Begin("Model");
    if(ImGui::TreeNode(uuid_.c_str())){
        ImGui::DragFloat3("Pos : ", &worldTransform_->translate.x, 0.1f);
        ImGui::DragFloat3("Rotate : ", &worldTransform_->rotate.x, 0.1f);
        ImGui::DragFloat3("Scale : ", &worldTransform_->scale.x, 0.1f);

        ImGui::TreePop();
    }
    ImGui::End();
#endif

    worldTransform_->Update();
}

void Model::Draw() const {
    if (!mesh_)return;

    commandList_->SetGraphicsRootConstantBufferView(1, worldTransform_->GetGPUVirtualAddress());
    mesh_->Draw();
}

void Model::SetMesh(const std::string& name) {
	mesh_ = ModelManager::GetInstance()->Find(name);

	assert(mesh_);
}
