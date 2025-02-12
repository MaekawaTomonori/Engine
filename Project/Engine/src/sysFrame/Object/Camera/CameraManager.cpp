#include "CameraManager.h"

#include <cassert>
#include <ranges>

#include "System/ImGui/ImGuiManager.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

CameraManager* CameraManager::instance_ = nullptr;
std::once_flag CameraManager::onceFlag_;

CameraManager* CameraManager::GetInstance() {
    std::call_once(onceFlag_, Create);
    assert(instance_);
    return instance_;
}

void CameraManager::Initialize() {
    Add("default");
    Active("default");
}

void CameraManager::Update() {
#ifdef _DEBUG
    ImGuiManager::GetInstance()->AddCommand(this, [&](){
        if (ImGui::Begin("CameraManager")){
            if (ImGui::CollapsingHeader("General")){
                if (ImGui::Button("Add")){
                    Add("noname" + std::to_string(cameras_.size()));
                }
            	for (auto& [name, camera] : cameras_){
                    if (ImGui::Selectable(name.c_str(), active_ == camera.get())){
                        Active(name);
                    }
                }
            }

            if (ImGui::CollapsingHeader("List")){
#endif
                for (auto& camera : cameras_ | std::views::values){
                    camera->Update();
                }
#if _DEBUG
            }

        }
        ImGui::End();
    });
#endif
}

Camera* CameraManager::GetCamera() const {
    return active_;
}

void CameraManager::Create() {
    instance_ = new CameraManager();
    SingletonFinalizer::AddFinalizer(&Destroy);
}

void CameraManager::Destroy() {
    delete instance_;
    instance_ = nullptr;
}

Camera* CameraManager::Add(const std::string& name) {
    if (cameras_.contains(name))return cameras_[name].get();

    cameras_[name] = std::make_unique<Camera>();
    cameras_[name]->Initialize();
    return cameras_[name].get();
}

Camera* CameraManager::Active(const std::string& name) {
    if (cameras_.contains(name)){
        active_ = cameras_[name].get();
    }

    return active_;
}
