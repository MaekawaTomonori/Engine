#pragma once
#include <memory>
#include <vector>
#include <d3d12.h>

#include "DirectX/DirectXCommon.h"
#include "System/Math/Transform.h"

#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "d3d12.lib")

struct VertexData;
struct Material;
struct TransformationMatrix;

static Transform CameraTransform = {
	{1,1,1},
	{0,0,0},
	{0,0,-5}
};

//抽象クラス
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

protected:
	//借り物 DirectXの情報をもらう
	DirectXCommon* dxCommon_ = nullptr;

	//借り物 
	ID3D12GraphicsCommandList* commandList_ = nullptr;

	//UUID
	std::string uuid_{};

	//Transform
	Transform transform_ {};

	///Resource類
	ComPtr<ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

    ComPtr<ID3D12Resource> materialResource_;

	ComPtr<ID3D12Resource> transformationMatrixResource_;

	ComPtr<ID3D12Resource> indexResource_;

	///ResourceにMappingするためのモノたち
	//VertexData
    VertexData* vertexData_ = nullptr;
	//Material
	Material* material_ = nullptr;
	TransformationMatrix* transformationMatrix_ = nullptr;

};

