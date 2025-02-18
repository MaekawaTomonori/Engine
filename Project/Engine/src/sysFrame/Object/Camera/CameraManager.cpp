#include "CameraManager.h"

#include <cassert>
#include <ranges>

#include "System/ImGui/ImGuiManager.h"
#include "System/Json/Json.h"
#include "System/Singleton/Singleton.h"
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
                char nameBuffer[256] = "";
                if (ImGui::InputText("Name", nameBuffer, IM_ARRAYSIZE(nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::Button("Add")){
                    Add(nameBuffer);
                    initialize = false;
                }

                // ActiveCamera
                if (!initialize){
                    for (const auto& name : cameras_ | std::views::keys){
                        names.push_back(name);
                    }
                    initialize = true;
                }

                if (ImGui::BeginCombo("ActiveCameras", names[currentIndex].c_str())){
                    for (auto& [name, camera]: cameras_){
	                    if(ImGui::Selectable(name.c_str(), active_ == camera.get())){
                            Active(name);
                            ImGui::SetItemDefaultFocus();
	                    }
                    }
                    ImGui::EndCombo();
                }
            }

            ImGui::Spacing();
            if (ImGui::CollapsingHeader("List")){
                for (auto& camera : cameras_ | std::views::values){
                    camera->ImGui();
                }
            }
        }
        ImGui::End();
    });
#endif

    for (auto& camera : cameras_ | std::views::values){
        camera->Update();
    }
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

void CameraManager::Add(const std::string& id, Camera* camera) {
    Json* json = Singleton<Json>::GetInstance();
    (void)json;
    (void)id;
    (void)camera;
}

void CameraManager::Load() {

}

void CameraManager::Save() {
}

Camera* CameraManager::Add(const std::string& name) {
    if (cameras_.contains(name))return cameras_[name].get();

    if (name.empty()){
        return Add("noname" + std::to_string(noname++));
    }
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

CameraManager::~CameraManager() {
}
