#pragma once
#include <d3d12.h>
#include <memory>
#include <wrl/client.h>

#include "ModelTransformation.h"
#include "Object/Camera/Camera.h"

class DirectXCommon;

class WorldTransform{
public:
	enum class Type{
		SPRITE,
        MODEL
	};

private:
	//借り物
    std::weak_ptr<DirectXCommon> dxCommon_;
	Camera* camera_ = nullptr;

    //2D or 3D
    Type type_;

    //Resource
    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

    //Mapping Data
    //間に合わせで2D3D両方置いている。後日修正する。
    TransformationMatrix* matrix_ = nullptr;
    TransformationMatrixForModel* matrixForModel_ = nullptr;

public://Variables
	//Transform transform_ {};

	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;

public: //Methods
	WorldTransform(const std::weak_ptr<DirectXCommon>& direx);
	void Initialize(Type type);
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

