#pragma once
#include <d3d12.h>
#include <memory>
#include <wrl/client.h>

#include "Transform.h"
#include "ModelTransformation.h"
#include "Object/Camera/Camera.h"

class DirectXCommon;

class WorldTransform{
    //借り物
    std::weak_ptr<DirectXCommon> dxCommon_;
	Camera* camera_ = nullptr;

    //Resource
    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

    //Mapping Data
    TransformationMatrix* matrix_ = nullptr;

public://Variables
	//Transform transform_ {};

	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;

public: //Methods
	WorldTransform(const std::weak_ptr<DirectXCommon>& direx);
	void Initialize();
    void Update() const;

    D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const {
        return resource_->GetGPUVirtualAddress();
    }

    void SetWVP(const Matrix4x4& m) const {
        matrix_->wvp = m;
    }

    void SetCamera(Camera* camera) {
        camera_ = camera;
    }
};

