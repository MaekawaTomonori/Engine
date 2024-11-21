#pragma once
#include <d3d12.h>
#include <wrl/client.h>

#include "DirectX/DirectXCommon.h"
#include "DirectX/ObjectCommon/ParticleCommon.h"
#include "Utility/Math/Transform.h"
#include "Utility/Math/TransformationMatrix.h"
#include "Utility/Math/Vector4.h"

class Camera;
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
    ParticleCommon* common_ = nullptr;

    Mesh* mesh_ = nullptr;

    Camera* camera_ = nullptr;

    static constexpr uint16_t MAX_COUNT = 50;
    const uint16_t SPAWN_COUNT = 3;

    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
    ParticleForGPU* forGpu_ = nullptr;
    std::array<std::unique_ptr<Particle>, MAX_COUNT> particle_;

    uint32_t srvIndex_ = 0;
    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_ {};

public:
	void Initialize(ParticleCommon* common, SRVManager* srv);
    void Update() const;
    void Draw() const;

    void SetCamera(Camera* camera);

private:
    static Particle Spawn();
};

