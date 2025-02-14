#include "LightManager.h"

#include <algorithm>
#include <fstream>

#include "magic_enum.hpp"
#include "DirectX/DirectXCommon.h"
#include "imgui/imgui.h"
#include "json/single_include/nlohmann/json.hpp"
#include "Object/Light/DirectionalLight/DirectionalLight.h"
#include "Object/Light/PointLight/PointLight.h"
#include "Object/Light/SpotLight/SpotLight.h"
#include "System/System.h"
#include "System/ImGui/ImGuiManager.h"
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

void LightManager::ImGui() {
    ImGuiManager::GetInstance()->AddCommand(this, [&](){
        if (ImGui::Begin("Light")){
            if (ImGui::BeginTabBar("Light")){
                if (ImGui::BeginTabItem("General")){
                    if(ImGui::CollapsingHeader("Files")){
                    	if(ImGui::Button("Load")){Load();}
                        ImGui::SameLine();
                        if(ImGui::Button("Save")){Save();}
                    }

                    ImGui::SeparatorText("Directional");
                    ImGui::Text("Light Count: %d", static_cast<int>(lightCount_->dlCount));
                    if (ImGui::Button("Add")){Add(LightType::Directional);}

                    ImGui::SeparatorText("Point");
                    ImGui::Text("Light Count: %d", static_cast<int>(lightCount_->plCount));
                    if (ImGui::Button("Add")){Add(LightType::Point);}

                    ImGui::SeparatorText("Spot");
                    ImGui::Text("Spot Light Count: %d", static_cast<int>(lightCount_->slCount));
                    if (ImGui::Button("Add")){Add(LightType::Spot);}
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Directional")){
                    for (auto& dl : rawDirectionalLights_){
                        dl->Update();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Point")){
                    for (auto& pl : rawPointLights_){
                        pl->Update();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Spot")){
                    for (auto& sl : rawSpotLights_){
                        sl->Update();
                    }
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
        }
    });
}

void LightManager::CheckState() {
    std::erase_if(rawDirectionalLights_, [&](const std::unique_ptr<RawDirectionalLight>& dl){
	    if (!dl->IsEnable()){
            lightCount_->dlCount--;
		    return true;
	    }
	    return false;
    });
    std::erase_if(rawPointLights_, [&](const std::unique_ptr<RawPointLight>& pl){
        if (!pl->IsEnable()){
            lightCount_->plCount--;
            return true;
        }
        return false;
    });
    std::erase_if(rawSpotLights_, [&](const std::unique_ptr<RawSpotLight>& sl){
        if (!sl->IsEnable()){
            lightCount_->slCount--;
            return true;
        }
        return false;
    });
}

void LightManager::Load() {
    std::ifstream file("Light.json");

    // File not found
    if (!file.good()){
        return;
    }

    nlohmann::json json = nlohmann::json::parse(file);
    auto lights = json.at("Light");
    rawDirectionalLights_.clear();
    rawPointLights_.clear();
    rawSpotLights_.clear();

    for(auto light : lights){
	    const auto type = magic_enum::enum_cast<LightType>(light.at("Type").get<std::string>()).value();
        Add(type);
        switch (type){ 
        case LightType::Directional:
            DirectionalLight dl;
            dl = {
	            .color = light.at("Color").get<Vector4>,
	            .direction = light.at("Direction").get<Vector3>,
		        .intensity = light.at("Intensity").get<float>
            };

            rawDirectionalLights_.back()->Set(dl);
            break;
        case LightType::Point:
            PointLight pl;
            pl = {
                .color = light.at("color").get<Vector4>,
                .position = light.at("position").get<Vector3>,
                .intensity = light.at("intensity").get<float>,
                .radius = light.at("radius").get<float>,
                .decay = light.at("decay").get<float>
            };
        	rawPointLights_.back()->Set(pl);
            break;
        case LightType::Spot:
            SpotLight sl = {
                .color = light.at("color").get<Vector4>,
                .position = light.at("position").get<Vector3>,
                .direction = light.at("direction").get<Vector3>,
                .distance = light.at("distance").get<float>,
                .intensity = light.at("intensity").get<float>,
                .decay = light.at("decay").get<float>,
                .cosAngle = light.at("cosAngle").get<float>,
                .falloffStart = light.at("falloffStart").get<float>
            };
            rawSpotLights_.back()->Set(sl);
        }
    }
}

void LightManager::Save() {
    nlohmann::json json;
    nlohmann::json lights;
    
    json["Light"] = lights;
    std::ofstream file("Light.json");
    file << json.dump(4);
}

void LightManager::Initialize(const std::weak_ptr<DirectXCommon>& dxCommon) {
    dxCommon_ = dxCommon;

    auto dxc = dxCommon_.lock();
    if (!dxc){
        return;
    }

    // Light Counter
	countResource_.Attach(DirectXCommon::CreateBufferResource(dxc->GetDevice(), sizeof(LightCount)).Get());
    countResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightCount_));

    lightCount_->dlCount = 0;
    lightCount_->plCount = 0;
    lightCount_->slCount = 0;

    //Directional
    directionalResource_.Attach(DirectXCommon::CreateBufferResource(dxc->GetDevice(), sizeof(DirectionalLight) * MAX_COUNT.dlCount).Get());
    directionalResource_->Map(0, nullptr, reinterpret_cast<void**>(&mdDirectional_));

    //Point
    pointResource_.Attach(DirectXCommon::CreateBufferResource(dxc->GetDevice(), sizeof(PointLight) * MAX_COUNT.plCount).Get());
    pointResource_->Map(0, nullptr, reinterpret_cast<void**>(&mdPointLight_));

    //Spot
    spotResource_.Attach(DirectXCommon::CreateBufferResource(dxc->GetDevice(), sizeof(SpotLight) * MAX_COUNT.slCount).Get());
    spotResource_->Map(0, nullptr, reinterpret_cast<void**>(&mdSpotLight_));

    Load();

    System::Log(Log::Level::INFO, "Light Enabled");
}

void LightManager::Update() {
    ImGui();
    CheckState();


    // Apply to GPU (raw data -mapping-> gpu data)
    uint32_t index = 0;
    for (index = 0; index < lightCount_->dlCount; ++index){
        mdDirectional_[index] = rawDirectionalLights_[index]->GetLight();
    }
    for (index = 0; index < lightCount_->plCount; ++index){
        mdPointLight_[index] = rawPointLights_[index]->GetLight();
    }
    for (index = 0; index < lightCount_->slCount; ++index){
        mdSpotLight_[index] = rawSpotLights_[index]->GetLight();
    }
}

void LightManager::Draw() const {
    auto dxc = dxCommon_.lock();
    if (!dxc){
        return;
    }

	dxc->GetCommandList()->SetGraphicsRootShaderResourceView(3, directionalResource_->GetGPUVirtualAddress());

    dxc->GetCommandList()->SetGraphicsRootShaderResourceView(5, pointResource_->GetGPUVirtualAddress());
    dxc->GetCommandList()->SetGraphicsRootShaderResourceView(6, spotResource_->GetGPUVirtualAddress());
    dxc->GetCommandList()->SetGraphicsRootConstantBufferView(7, countResource_->GetGPUVirtualAddress());
}

void LightManager::Add(LightType type) {
    std::unique_ptr<RawDirectionalLight> directional;
    std::unique_ptr<RawPointLight> point;
    std::unique_ptr<RawSpotLight> spot;

	switch (type){
	case LightType::Directional:
        if (MAX_COUNT.dlCount <= ++lightCount_->dlCount){
            return;
        }
        directional = std::make_unique<RawDirectionalLight>();
        directional->DefaultSetting();
        rawDirectionalLights_.push_back(std::move(directional));
		break;
	case LightType::Point:
        if (MAX_COUNT.plCount <= ++lightCount_->plCount){
            return;
        }
        point = std::make_unique<RawPointLight>();
        point->DefaultSetting();
        rawPointLights_.push_back(std::move(point));
		break;
	case LightType::Spot:
        if (MAX_COUNT.slCount <= ++lightCount_->slCount){
            return;
        }
        spot = std::make_unique<RawSpotLight>();
        spot->DefaultSetting();
        rawSpotLights_.push_back(std::move(spot));
		break;
	}
}
