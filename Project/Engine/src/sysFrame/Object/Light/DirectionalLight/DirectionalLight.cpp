#include "DirectionalLight.h"

#include "imgui/imgui.h"

void RawDirectionalLight::DefaultSetting() {
    type_ = LightType::Directional;
    light_.color = {1, 1, 1, 1};
    light_.direction = {0, -1, 0};
    light_.intensity = 1.f;
}

void RawDirectionalLight::ImGuiSetting() {
    if (ImGui::TreeNode(uuid_.c_str())){
        ImGui::ColorEdit4("Color", &light_.color.x);
        ImGui::DragFloat3("Direction", &light_.direction.x, 0.1f);
        ImGui::DragFloat("Intensity", &light_.intensity, 0.01f, 0, 1);


        if (ImGui::Button("Delete")){
            enable_ = false;
        }
        ImGui::TreePop();
    }

    light_.direction.Normalize();
}
