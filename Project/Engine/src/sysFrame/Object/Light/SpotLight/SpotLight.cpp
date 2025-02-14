#include "SpotLight.h"

#include "imgui.h"
#include "System/Math/MathUtils.h"

void RawSpotLight::DefaultSetting() {
    type_ = LightType::Spot;
    light_.color = {1,1,1,1};
    light_.position = {2.f, 1.25f, 0.f};
    light_.distance = 7.f;
    light_.direction = Vector3(-1.f, -1.f, 0).Normalize();
    light_.intensity = 0.f;
    light_.decay = 2.f;
    light_.cosAngle = std::cos(MathUtils::F_PI / 3.f);
    light_.falloffStart = std::cos(MathUtils::F_PI / 4.f);
}

void RawSpotLight::ImGuiSetting() {
    if (ImGui::TreeNode(uuid_.c_str())){

        ImGui::ColorEdit4("Color", &light_.color.x);
        ImGui::DragFloat3("Position", &light_.position.x, 0.1f);
        ImGui::DragFloat3("Direction", &light_.direction.x, 0.1f);
        ImGui::DragFloat("Distance", &light_.distance, 0.1f, 0.f);
        ImGui::DragFloat("Intensity", &light_.intensity, 0.01f, 0.f, 10.f);
        ImGui::DragFloat("decay", &light_.decay, 0.01f, 0.f);
        ImGui::DragFloat("cosAngle", &light_.cosAngle, 0.01f, light_.falloffStart);
        ImGui::DragFloat("falloffStart", &light_.falloffStart, 0.01f, 0.f);

	    if (ImGui::Button("Delete")){
	        enable_ = false;
	    }
        ImGui::TreePop();
    }


    if ((MathUtils::F_PI * 2.f) <= light_.cosAngle){
        light_.cosAngle -= MathUtils::F_PI * 2.f;
    }
    if ((MathUtils::F_PI * 2.f) <= light_.falloffStart){
        light_.falloffStart -= MathUtils::F_PI * 2.f;
    }

    if (light_.falloffStart < light_.cosAngle){
        light_.falloffStart = light_.cosAngle + std::cos(MathUtils::F_PI / 10.f);
    }
    light_.direction.Normalize();
}