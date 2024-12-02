#include "LightManager.h"

#include "DirectX/DirectXCommon.h"
#include "imgui/imgui.h"
#include "Object/Light/DirectionalLight/DirectionalLight.h"
#include "Object/Light/PointLight/PointLight.h"

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

    pointResource_.Attach(DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(PointLight)));
    pointResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLight_));

    pointLight_->color = {1,1,1,1};
    pointLight_->position = {0,2,0};
    pointLight_->intensity = 1.f;
    pointLight_->radius = 10;
    pointLight_->decay = 1;
}

void LightManager::Update() const {
#ifdef _DEBUG
    ImGui::Begin("Light");
    if (ImGui::TreeNode("Directional")){
        ImGui::ColorEdit4("Color", &directionalLight_->color.x);
        ImGui::DragFloat3("Direction", &directionalLight_->direction.x, 0.1f);
        ImGui::DragFloat("Intensity", &directionalLight_->intensity, 0.01f, 0, 1);

        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Point")){
        ImGui::ColorEdit4("Color", &pointLight_->color.x);
        ImGui::DragFloat3("Position", &pointLight_->position.x, 0.1f);
        ImGui::DragFloat("Intensity", &pointLight_->intensity, 0.01f, 0, 1);
        ImGui::DragFloat("radius", &pointLight_->radius, 0.01f);
        ImGui::DragFloat("decay", &pointLight_->decay, 0.01f);

        ImGui::TreePop();
    }
    ImGui::End();
#endif


    directionalLight_->direction.normalize();
}

void LightManager::Draw() const {
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalResource_->GetGPUVirtualAddress());
    dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(5, pointResource_->GetGPUVirtualAddress());
}
