#pragma once
#include "ParticleCommon.h"
#include "DirectX/Heap/SRVManager.h"
#include "System/Math/Matrix.h"
#include "System/Math/Vector2.h"
#include "System/Math/Vector4.h"

#include "Particle2d.h"
#include "ParticleManager.h"


class Emitter2d {
	struct VertexData{
		Vector4 position;
        Vector2 texcoord;
	};

	struct ForGpu{
		Matrix4x4 wvp;
        Vector4 color;
	};

    static uint32_t constexpr MAX_PARTICLE = 100;
    uint32_t activeCount_ = 0;

	ParticleCommon* pCommon_ = nullptr;
    SRVManager* srvManager_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList_;

    std::string texture_;
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ {};
    Microsoft::WRL::ComPtr<ID3D12Resource> dataResource_;

    VertexData* vertexData_ = nullptr;

	ComPtr<ID3D12Resource> indexResource_;
    D3D12_INDEX_BUFFER_VIEW indexBufferView_ {};
    uint32_t* indexData_ = nullptr;

    std::vector<std::unique_ptr<Particle2d>> particles_;
    ForGpu* data_ = nullptr;

    uint32_t index_ = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_{};

    //emitter data
    EmitterConfig config_;


public:
	Emitter2d(ParticleCommon* pCommon, SRVManager* srvManager) : pCommon_(pCommon), srvManager_(srvManager), config_() {}

    ~Emitter2d() = default;

	void Initialize();
	void Update() const;
	void Draw();

	void Emit();

    void SetTexture(const std::string& texture) {
        texture_ = texture;
    }

    void SetConfig(const EmitterConfig& config) {
        config_ = config;
    }
	EmitterConfig GetConfig() const {
		return config_;
	}
};

