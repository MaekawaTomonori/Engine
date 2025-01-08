#pragma once
#include <memory>
#include <string>

#include "ModelCommon.h"
#include "Object/Camera/Camera.h"
#include "System/Math/WorldTransform.h"

class DirectXCommon;
class Mesh;

class Model{
private:
	///借りポ
	ModelCommon* modelCommon_ = nullptr;

    std::weak_ptr<DirectXCommon> dxCommon_;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;

    Mesh* mesh_ = nullptr;

    Camera* camera_ = nullptr;

    //Member

	std::string uuid_ {};


    std::unique_ptr<WorldTransform> worldTransform_ = nullptr;


    Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource_;
    CameraForGPU* cameraForGPU_ = nullptr;

public:
	Model() :modelCommon_(ModelCommon::GetInstance()) {
        UUID uuid;
        UuidCreate(&uuid);
        RPC_CSTR szUuid = nullptr;
        UuidToStringA(&uuid, &szUuid);
        uuid_ = reinterpret_cast<char*>(szUuid);
        RpcStringFreeA(&szUuid);
	}
    ~Model();

    void Initialize();
    void Update() const;
    void Draw() const;
    void Draw(const std::string& texture) const;

    void SetMesh(const std::string& name);

	void SetCamera(Camera* camera) {
        camera_ = camera;
        worldTransform_->SetCamera(camera_);
    }

    void SetTransform(const Transform& transform) const;

    void SetTexture(const std::string& name) const;
};

