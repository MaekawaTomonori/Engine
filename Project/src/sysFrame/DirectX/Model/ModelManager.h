#pragma once
#include <map>
#include <memory>
#include <mutex>
#include <string>

class DirectXCommon;
class Mesh;
class MeshCommon;

//ModelManager ? MeshManager
class ModelManager{
    static ModelManager* instance_;
    static std::once_flag onceFlag_;
	static void Create();

    ModelManager() = default;
    ~ModelManager();

private://Variables
	std::map<std::string, std::shared_ptr<Mesh>> models_;
    std::shared_ptr<MeshCommon> meshCommon_ = nullptr;

    std::string folderPath_ = "assets/Resources/";

public:
    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;

    static ModelManager* GetInstance();

public:
    static void Finalize();
	void Initialize(std::weak_ptr<DirectXCommon> dxCommon);

    void Load(const std::string& fileName);

    Mesh* Find(const std::string& name);
};

