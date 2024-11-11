#include "LightManager.h"

#include "DirectX/DirectXCommon.h"
#include "imgui/imgui.h"
#include "Object/Light/DirectionalLight/DirectionalLight.h"

std::shared_ptr<LightManager> LightManager::instance = nullptr;

std::shared_ptr<LightManager> LightManager::GetInstance() {
    if (!instance){
        instance = std::shared_ptr<LightManager>(new LightManager, [](const LightManager* ptr) {
            delete ptr;
        });
    }

    return instance;
}

void LightManager::Initialize(DirectXCommon* dxCommon) {
    dxCommon_ = dxCommon;

    //Directional
    directionalResource_.Attach(DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(DirectionalLight)));


    directionalResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLight_));

    directionalLight_->color = {1, 1, 1, 1};
    directionalLight_->direction = {0, -1, 0};
    directionalLight_->intensity = 1.f;
}

void LightManager::Update() const {
    ImGui::Begin("Light");
    if (ImGui::TreeNode("Directional")){
        ImGui::ColorEdit4("Color", &directionalLight_->color.x);
        ImGui::DragFloat3("Direction", &directionalLight_->direction.x, 0.1f);
        ImGui::DragFloat("Intensity", &directionalLight_->intensity, 0.1f);

        ImGui::TreePop();
    }
    ImGui::End();

    directionalLight_->direction.normalize();
}

void LightManager::Draw(LightType type) const {
    switch (type){
        case LightType::Directional:
		default:
            dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalResource_->GetGPUVirtualAddress());
            break;
    }
}
