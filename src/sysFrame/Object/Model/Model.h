#pragma once
#include <memory>
#include <string>

#include "DirectX/Model/ModelManager.h"
#include "System/Math/WorldTransform.h"

class DirectXCommon;
class Mesh;
class ModelCommon;

class Model{
private:
	///借りポ
	ModelCommon* modelCommon_ = nullptr;

    DirectXCommon* dxCommon_ = nullptr;
    ID3D12GraphicsCommandList* commandList_ = nullptr;

    Mesh* mesh_ = nullptr;

    Camera* camera_ = nullptr;

    //Member

	std::string uuid_ {};


    std::shared_ptr<WorldTransform> worldTransform_ = nullptr;


public:
	Model(ModelCommon* modelCommon) :modelCommon_(modelCommon) {
        UUID uuid;
        UuidCreate(&uuid);
        RPC_CSTR szUuid = nullptr;
        UuidToStringA(&uuid, &szUuid);
        uuid_ = reinterpret_cast<char*>(szUuid);
        RpcStringFreeA(&szUuid);
	}

    void Initialize();
    void Update();
    void Draw();

    void SetMesh(const std::string& name);

	void SetCamera(Camera* camera) {
        camera_ = camera;
        worldTransform_->SetCamera(camera_);
    }
};

