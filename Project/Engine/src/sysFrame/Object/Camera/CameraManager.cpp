#include "CameraManager.h"

#include <cassert>

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
    cameras_[name] = std::make_unique<Camera>();
    cameras_[name]->Initialize();
    return cameras_[name].get();
}

Camera* CameraManager::Active(const std::string& name) {
    active_ = cameras_[name].get();
    return active_;
}
