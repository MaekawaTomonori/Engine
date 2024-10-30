#pragma once
#include <memory>
#include <d3d12.h>

#include "DirectX/DirectXCommon.h"
#include "System/Math/Material.h"
#include "System/Math/Transform.h"
#include "System/Math/VertexData.h"
#include "System/Math/WorldTransform.h"

#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "d3d12.lib")

class Camera;

//抽象クラス 3D
class Object{
public:
	Object(DirectXCommon* dxCommon) {
		dxCommon_ = dxCommon;
		commandList_ = dxCommon_->GetCommandList();

		UUID uuid;
		UuidCreate(&uuid);
		RPC_CSTR szUuid = nullptr;
		UuidToStringA(&uuid, &szUuid);
		uuid_ = reinterpret_cast<char*>(szUuid);
		RpcStringFreeA(&szUuid);
	}
	virtual ~Object() = default;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

    void SetCamera(Camera* camera) {
        camera_ = camera;
    }

protected:
	//借り物 DirectXの情報をもらう
	DirectXCommon* dxCommon_ = nullptr;

	//借り物 
	ID3D12GraphicsCommandList* commandList_ = nullptr;

	//UUID
	std::string uuid_{};

	std::string texturePath;

	///Resource類
	
	//VertexData
	ComPtr<ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
    VertexData* vertexData_ = nullptr;

	//Index
	ComPtr<ID3D12Resource> indexResource_;
    D3D12_INDEX_BUFFER_VIEW indexBufferView_ {};
    uint32_t* indexData_ = nullptr;

	//Material
    ComPtr<ID3D12Resource> materialResource_;
	Material* material_ = nullptr;

    //WorldTransform
	std::shared_ptr<WorldTransform> worldTransform_;

	//カメラ情報 @Nullable
    Camera* camera_ = nullptr;
};

