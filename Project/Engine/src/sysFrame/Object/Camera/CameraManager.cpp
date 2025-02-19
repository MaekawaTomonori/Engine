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
    Load();

    if (cameras_.empty()){
        Add("Default");
        Active("Default");
    }
}

void CameraManager::Update() {
#ifdef _DEBUG
    ImGuiManager::GetInstance()->AddCommand(this, [&](){
        if (ImGui::Begin("CameraManager")){
            if (ImGui::CollapsingHeader("General")){
                //Load Save
                if (ImGui::Button("Load / Reload")){Load();}
                ImGui::SameLine();
                if (ImGui::Button("Save")){Save();}

                ImGui::Separator();

                char nameBuffer[256] = "";
                bool entry = ImGui::InputTextWithHint("##NamePicker", "Name", nameBuffer, IM_ARRAYSIZE(nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);
                ImGui::SameLine();
                if (entry || ImGui::Button("Add")){
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

                ImGui::Text("ActiveCamera");
                ImGui::SameLine();
                if (ImGui::BeginCombo("##combo", names[currentIndex].c_str())){
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

void CameraManager::Load() {
    active_ = nullptr;
    cameras_.clear();

    Json* json = Singleton<Json>::GetInstance();
    if (!json->Load("Camera")) return;

    auto group = json->GetGroups("Camera");
    for (auto& [groupId, object] : group){
        Camera* camera = Add(groupId);
        camera->transform_ = {
            {1,1,1},
        	std::get<Vector3>(object["Rotate"]),
            std::get<Vector3>(object["Position"])
        };
    }
    Active(cameras_.begin()->first);
}

void CameraManager::Save() {
    Json* json = Singleton<Json>::GetInstance();
    for (auto& [name, camera] : cameras_){
        json->SetValue("Camera", name, "Position", camera->transform_.translate);
        json->SetValue("Camera", name, "Rotate", camera->transform_.rotate);
    }
    json->Save("Camera");
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
    Save();
}
