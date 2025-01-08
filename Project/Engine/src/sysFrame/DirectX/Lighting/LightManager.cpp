#include "LightManager.h"

#include "DirectX/DirectXCommon.h"
#include "imgui/imgui.h"
#include "Object/Light/DirectionalLight/DirectionalLight.h"
#include "Object/Light/PointLight/PointLight.h"
#include "Object/Light/SpotLight/SpotLight.h"
#include "System/System.h"
#include "System/Math/MathUtils.h"
#include "System/Math/Vector3.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

LightManager* LightManager::instance = nullptr;
std::once_flag LightManager::onceFlag_;

LightManager* LightManager::GetInstance() {
    std::call_once(onceFlag_, Create);
    assert(instance);
    return instance;
}

void LightManager::Create() {
    instance = new LightManager();
    SingletonFinalizer::AddFinalizer(&Finalize);
}

void LightManager::Finalize() {
    delete instance;
    instance = nullptr;
    System::Log(Log::Level::INFO, "Light Disabled");
}

void LightManager::Initialize(const std::weak_ptr<DirectXCommon>& dxCommon) {
    dxCommon_ = dxCommon;

    auto dxc = dxCommon_.lock();
    if (!dxc){
        return;
    }

    //Directional
    directionalResource_.Attach(DirectXCommon::CreateBufferResource(dxc->GetDevice(), sizeof(DirectionalLight)).Get());


    directionalResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLight_));

    directionalLight_->color = {1, 1, 1, 1};
    directionalLight_->direction = {0, -1, 0};
    directionalLight_->intensity = 1.f;

    pointResource_.Attach(DirectXCommon::CreateBufferResource(dxc->GetDevice(), sizeof(PointLight)).Get());
    pointResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLight_));

    pointLight_->color = {1,1,1,1};
    pointLight_->position = {0,2,0};
    pointLight_->intensity = 0.f;
    pointLight_->radius = 10;
    pointLight_->decay = 1;

    spotResource_.Attach(DirectXCommon::CreateBufferResource(dxc->GetDevice(), sizeof(SpotLight)).Get());
    spotResource_->Map(0, nullptr, reinterpret_cast<void**>(&spotLight_));

    spotLight_->color = {1,1,1,1};
    spotLight_->position = {2.f, 1.25f, 0.f};
    spotLight_->distance = 7.f;
    spotLight_->direction = Vector3(-1.f, -1.f, 0).normalize();
    spotLight_->intensity = 0.f;
    spotLight_->decay = 2.f;
    spotLight_->cosAngle = std::cos(MathUtils::F_PI / 3.f);
    spotLight_->falloffStart = std::cos(MathUtils::F_PI / 4.f);

    System::Log(Log::Level::INFO, "Light Enabled");
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

    if (ImGui::TreeNode("Spot")){
        ImGui::ColorEdit4("Color", &spotLight_->color.x);
        ImGui::DragFloat3("Position", &spotLight_->position.x, 0.1f);
        ImGui::DragFloat3("Direction", &spotLight_->direction.x, 0.1f);
        ImGui::DragFloat("Distance", &spotLight_->distance, 0.1f, 0.f);
    	ImGui::DragFloat("Intensity", &spotLight_->intensity, 0.01f, 0.f, 10.f);
        ImGui::DragFloat("decay", &spotLight_->decay, 0.01f, 0.f);
        ImGui::DragFloat("cosAngle", &spotLight_->cosAngle, 0.01f, spotLight_->falloffStart);
        ImGui::DragFloat("falloffStart", &spotLight_->falloffStart, 0.01f, 0.f);

        ImGui::TreePop();
    }
    ImGui::End();
#endif


    directionalLight_->direction.normalize();

    if ((MathUtils::F_PI * 2.f) <= spotLight_->cosAngle){
        spotLight_->cosAngle -= MathUtils::F_PI * 2.f;
    }
	if ((MathUtils::F_PI * 2.f) <= spotLight_->falloffStart){
        spotLight_->falloffStart -= MathUtils::F_PI * 2.f;
    }

    if (spotLight_->falloffStart < spotLight_->cosAngle){
		spotLight_->falloffStart = spotLight_->cosAngle + std::cos(MathUtils::F_PI / 10.f);
    }

    spotLight_->direction.normalize();
}

void LightManager::Draw() const {
    auto dxc = dxCommon_.lock();
    if (!dxc){
        return;
    }

	dxc->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalResource_->GetGPUVirtualAddress());
    dxc->GetCommandList()->SetGraphicsRootConstantBufferView(5, pointResource_->GetGPUVirtualAddress());
    dxc->GetCommandList()->SetGraphicsRootConstantBufferView(6, spotResource_->GetGPUVirtualAddress());
}
