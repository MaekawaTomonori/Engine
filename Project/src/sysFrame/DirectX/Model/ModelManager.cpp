#include "ModelManager.h"

#include <cassert>
#include <threads.h>

#include "DirectX/DirectXCommon.h"
#include "DirectX/ObjectCommon/MeshCommon.h"
#include "Object/Model/Mesh/Mesh.h"
#include "System/System.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

ModelManager* ModelManager::instance_ = nullptr;
std::once_flag ModelManager::onceFlag_;

ModelManager* ModelManager::GetInstance() {
    call_once(onceFlag_, Create);
    assert(instance_);
    return instance_;
}

void ModelManager::Create() {
    instance_ = new ModelManager();
    SingletonFinalizer::AddFinalizer(&Finalize);
}

ModelManager::~ModelManager() {
    models_.clear();
}

void ModelManager::Finalize() {
    delete instance_;
    instance_ = nullptr;
    System::Log(Log::Level::INFO, "ModelManager Disabled");
}

void ModelManager::Initialize(std::weak_ptr<DirectXCommon> dxCommon) {
    meshCommon_ = std::make_shared<MeshCommon>(dxCommon);
    meshCommon_->Initialize();

    System::Log(Log::Level::INFO, "ModelManager Enabled");
}

void ModelManager::Load(const std::string& fileName) {
    if (models_.contains(fileName))return;

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(meshCommon_.get());
    mesh->Initialize(folderPath_, fileName);

    models_[fileName] = mesh;

    System::Log(Log::Level::INFO, std::format("Model Loaded:{}", fileName));
}

Mesh* ModelManager::Find(const std::string& name) {
    if (models_.contains(name)){
        return models_[name].get();
    }

    System::Log(Log::Level::ERR, std::format("Model Not Found:{}", name));
    return nullptr;
}

