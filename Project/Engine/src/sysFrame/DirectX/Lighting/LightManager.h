#pragma once
#include <d3d12.h>
#include <memory>
#include <mutex>
#include <vector>
#include <wrl/client.h>

class RawPointLight;
class RawDirectionalLight;
class RawSpotLight;
struct DirectionalLight;
struct PointLight;
struct SpotLight;

class DirectXCommon;

enum class LightType{
	Directional,
    Point,
    Spot
};

class LightManager final{
	struct LightCount{
        uint32_t dlCount;
        uint32_t plCount;
        uint32_t slCount;
	};

    std::weak_ptr<DirectXCommon> dxCommon_;

    Microsoft::WRL::ComPtr<ID3D12Resource> directionalResource_;
    DirectionalLight* mdDirectional_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> pointResource_;
    PointLight* mdPointLight_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> spotResource_;
    SpotLight* mdSpotLight_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> countResource_;
    LightCount* lightCount_ = nullptr;

	static LightManager* instance;
    static std::once_flag onceFlag_;

    const LightCount MAX_COUNT {20, 20, 20};

    std::vector<std::unique_ptr<RawDirectionalLight>> rawDirectionalLights_;
    std::vector<std::unique_ptr<RawPointLight>> rawPointLights_;
    std::vector<std::unique_ptr<RawSpotLight>> rawSpotLights_;

public:
	LightManager(const LightManager&) = delete;
    void operator=(const LightManager&) = delete;
    static LightManager* GetInstance();

	void Initialize(const std::weak_ptr<DirectXCommon>& dxCommon);
    void Update();
    void Draw() const;

    void Add(LightType type);

private:
    LightManager() = default;
    ~LightManager() = default;
    static void Create();
    static void Finalize();

    void ImGui();
    void CheckState();

    void Load();
    void Save();
};

