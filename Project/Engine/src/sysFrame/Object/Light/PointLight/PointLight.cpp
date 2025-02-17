#include "PointLight.h"

#include "imgui/imgui.h"

void RawPointLight::DefaultSetting() {
    type_ = LightType::Point;
	light_.color = {1, 1, 1, 1};
	light_.position = {0, 2, 0};
	light_.intensity = 0.f;
	light_.radius = 10;
	light_.decay = 1;
}

void RawPointLight::Set(const PointLight& pl) {
    light_ = pl;
}

void RawPointLight::ImGuiSetting() {
 
	if (ImGui::TreeNode(uuid_.c_str())){
	    ImGui::ColorEdit4("Color", &light_.color.x);
	    ImGui::DragFloat3("Position", &light_.position.x, 0.1f);
	    ImGui::DragFloat("Intensity", &light_.intensity, 0.01f, 0, 1);
	    ImGui::DragFloat("radius", &light_.radius, 0.01f);
	    ImGui::DragFloat("decay", &light_.decay, 0.01f);

        if (ImGui::Button("Delete")){
            enable_ = false;
        }
        ImGui::TreePop();
    }
}
