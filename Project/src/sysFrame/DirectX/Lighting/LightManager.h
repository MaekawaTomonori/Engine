#pragma once
#include <d3d12.h>
#include <memory>
#include <wrl/client.h>

struct SpotLight;
struct DirectionalLight;
struct PointLight;
class DirectXCommon;

enum class LightType{
	Directional,
};

class LightManager final{
    DirectXCommon* dxCommon_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> directionalResource_;
    DirectionalLight* directionalLight_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> pointResource_;
    PointLight* pointLight_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> spotResource_;
    SpotLight* spotLight_ = nullptr;

    LightManager() = default;
    ~LightManager() = default;


	static std::shared_ptr<LightManager> instance;

public:
	LightManager(const LightManager&) = delete;
    void operator=(const LightManager&) = delete;
    void Finalize();

    static std::shared_ptr<LightManager> GetInstance();

	void Initialize(DirectXCommon* dxCommon);
    void Update() const;
    void Draw() const;
};

