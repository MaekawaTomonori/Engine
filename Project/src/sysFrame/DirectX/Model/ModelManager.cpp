#include "ModelManager.h"

#include <cassert>

#include "DirectX/DirectXCommon.h"
#include "DirectX/ObjectCommon/MeshCommon.h"
#include "Object/Model/Mesh/Mesh.h"
#include "System/System.h"

std::shared_ptr<ModelManager> ModelManager::instance_ = nullptr;

void ModelManager::InstanceInit() {
    instance_ = std::shared_ptr<ModelManager>(new ModelManager, [](const ModelManager* ptr){
        delete ptr;
    });
}

void ModelManager::Finalize() {
    models_.clear();

    System::Log(Log::Level::INFO, "ModelManager Disabled");
}

std::shared_ptr<ModelManager> ModelManager::GetInstance() {
    if(!instance_){
        InstanceInit();
    }

    return instance_;
}

void ModelManager::Initialize(DirectXCommon* dxCommon) {
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

