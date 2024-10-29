#pragma once
#include <d3d12.h>
#include <wrl/client.h>

#include "Transform.h"
#include "TransformationMatrix.h"
#include "Object/Camera/Camera.h"

class DirectXCommon;

class WorldTransform{
    //借り物
    DirectXCommon* dxCommon_ = nullptr;
	Camera* camera_ = nullptr;

    //Resource
    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

    //Mapping Data
    TransformationMatrix* matrix_ = nullptr;

public://Variables
	Transform transform_ {};

public: //Methods
	WorldTransform(DirectXCommon* direx);
	void Initialize();
    void Update();

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

