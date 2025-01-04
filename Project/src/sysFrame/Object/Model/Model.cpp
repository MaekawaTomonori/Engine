#include "Model.h"

#include "ModelCommon.h"
#include "DirectX/DirectXCommon.h"
#include "Engine/Engine.h"
#include "Mesh/Mesh.h"

Model::~Model() = default;

void Model::Initialize() {
    dxCommon_ = modelCommon_->GetDXCommon();
    commandList_ = dxCommon_.lock()->GetCommandList();

    worldTransform_ = std::make_unique<WorldTransform>(dxCommon_);
    worldTransform_->Initialize();

    camera_ = Engine::GetDefaultCamera();

	cameraResource_.Attach(DirectXCommon::CreateBufferResource(dxCommon_.lock()->GetDevice(), sizeof(CameraForGPU)).Get());
    cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPU_));

    *cameraForGPU_ = camera_->GetCameraForGPU();

}

void Model::Update() const {
#ifdef _DEBUG
    ImGui::Begin("Model");
    if(ImGui::TreeNode(uuid_.c_str())){
        ImGui::DragFloat3("Pos", &worldTransform_->translate.x, 0.1f);
        ImGui::DragFloat3("Rotate", &worldTransform_->rotate.x, 0.1f);
        ImGui::DragFloat3("Scale", &worldTransform_->scale.x, 0.1f);
        ImGui::ColorEdit4("Color", &mesh_->GetColor().x);
        mesh_->ImGuiAccess();
        ImGui::TreePop();
    }
    ImGui::End();
#endif

    worldTransform_->Update();
}

void Model::Draw() const {
    if (!mesh_)return;
    modelCommon_->PreDraw();

    commandList_->SetGraphicsRootConstantBufferView(1, worldTransform_->GetGPUVirtualAddress());
    commandList_->SetGraphicsRootConstantBufferView(4, cameraResource_->GetGPUVirtualAddress());
    mesh_->Draw();
}

void Model::SetMesh(const std::string& name) {
	mesh_ = ModelManager::GetInstance()->Find(name);

	assert(mesh_);
}
