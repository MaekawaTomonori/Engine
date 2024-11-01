#pragma once
#include <map>
#include <memory>
#include <string>


class DirectXCommon;
class Mesh;
class MeshCommon;

//ModelManager ? MeshManager
class ModelManager{
    static std::shared_ptr<ModelManager> instance_;
	static void InstanceInit();

    ModelManager() = default;
    ~ModelManager() = default;

private://Variables
	std::map<std::string, std::shared_ptr<Mesh>> models_;
    std::shared_ptr<MeshCommon> meshCommon_ = nullptr;

    std::string folderPath_ = "assets/Resources/";

public:
    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;

    static std::shared_ptr<ModelManager> GetInstance();

public:
	void Initialize(DirectXCommon* dxCommon);

    void Load(const std::string& fileName);

    Mesh* Find(const std::string& name);
};

