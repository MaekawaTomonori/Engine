#pragma once
#include <d3d12.h>
#include <memory>
#include <wrl/client.h>

struct DirectionalLight;
class DirectXCommon;

enum class LightType{
	Directional,
};

class LightManager final{
    DirectXCommon* dxCommon_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> directionalResource_;

    DirectionalLight* directionalLight_ = nullptr;

    LightManager() = default;
    ~LightManager() = default;


	static std::shared_ptr<LightManager> instance;

public:
	LightManager(const LightManager&) = delete;
    void operator=(const LightManager&) = delete;

	static std::shared_ptr<LightManager> GetInstance();

	void Initialize(DirectXCommon* dxCommon);
    void Update() const;
    void Draw(LightType type) const;
};

