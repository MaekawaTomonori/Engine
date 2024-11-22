#pragma once
#include <d3d12.h>
#include <list>
#include <wrl/client.h>

#include "AccelerationField.h"
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
    float lifeTime;
    float currentTime;
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

    Transform transform_ {};

    const uint16_t MAX_COUNT = 100;
    const uint16_t SPAWN_COUNT = 3;

    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
    ParticleForGPU* forGpu_ = nullptr;
    std::list<std::unique_ptr<Particle>> particle_;

    uint32_t srvIndex_ = 0;
    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_ {};

    uint16_t instanceCount = 0;

    Matrix4x4 backToFront{};

    float frequency_ = 0;
    float frequencyTime = 0;

    AccelerationField field_{};

public:
	void Initialize(ParticleCommon* common, SRVManager* srv, const Transform& transform);
    void Update();
    void Draw() const;

    void SetCamera(Camera* camera);

    void SetFrequency(float frequency);

    void SetTexture(const std::string& textureName) const;

private:
    std::list<std::unique_ptr<Particle>> Spawn() const;
};

