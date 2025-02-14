#include "LightManager.h"

#include <algorithm>

#include "DirectX/DirectXCommon.h"
#include "imgui/imgui.h"
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
                    ImGui::SeparatorText("Directional");
                    ImGui::Text("Light Count: %d", static_cast<int>(lightCount_->dlCount));

                    ImGui::SeparatorText("Point");
                    ImGui::Text("Light Count: %d", static_cast<int>(lightCount_->plCount));

                    ImGui::SeparatorText("Spot");
                    ImGui::Text("Spot Light Count: %d", static_cast<int>(lightCount_->slCount));
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Directional")){
                    if (ImGui::Button("Add")){Add(LightType::Directional);}
                    for (auto& dl : rawDirectionalLights_){
                        dl->Update();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Point")){
                    if (ImGui::Button("Add")){Add(LightType::Point);}
                    for (auto& pl : rawPointLights_){
                        pl->Update();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Spot")){
                    if (ImGui::Button("Add")){Add(LightType::Spot);}
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

    Add(LightType::Directional);
    Add(LightType::Point);
    Add(LightType::Spot);

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
