#pragma once
#include <d3d12.h>
#include <vector>
#include <wrl/client.h>

#include "DirectX/DirectXCommon.h"
#include "System/Math/Transform.h"
#include "System/Math/TransformationMatrix.h"
#include "System/Math/Vector4.h"

class Mesh;
class WorldTransform;
class SRVManager;

struct Particle{
    Transform transform;
    Vector3 velocity;
    Vector4 color;
};

struct ParticleForGPU{
    Matrix4x4 WVP;
    Matrix4x4 World;
    Vector4 Color;
};

class Emitter{
    ID3D12GraphicsCommandList* commandList_ = nullptr;

    Mesh* mesh_ = nullptr;

    const uint16_t MAX_COUNT = 10;
    const uint16_t SPAWN_COUNT = 3;

    Microsoft::WRL::ComPtr<ID3D12Resource> transformation_;
    Particle* particle_ = nullptr;

    uint32_t srvIndex_ = 0;
    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_ {};

public:
	void Initialize(const DirectXCommon* dxCommon, SRVManager* srv);
    void Update();
    void Draw() const;

private:
	Particle Spawn();
};

