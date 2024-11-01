#include "ModelManager.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/ObjectCommon/MeshCommon.h"
#include "Object/Model/Mesh/Mesh.h"

std::shared_ptr<ModelManager> ModelManager::instance_ = nullptr;

void ModelManager::InstanceInit() {
    instance_ = std::shared_ptr<ModelManager>(new ModelManager, [](const ModelManager* ptr){
        delete ptr;
    });
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
}

void ModelManager::Load(const std::string& fileName) {
    if (models_.contains(fileName))return;

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(meshCommon_.get());
    mesh->Initialize(folderPath_, fileName);

    models_[fileName] = mesh;
}

Mesh* ModelManager::Find(const std::string& name) {
    if (models_.contains(name)){
        return models_[name].get();
    }

    return nullptr;
}

