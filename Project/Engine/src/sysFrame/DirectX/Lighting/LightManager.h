#pragma once
#include <d3d12.h>
#include <memory>
#include <mutex>
#include <wrl/client.h>

struct SpotLight;
struct DirectionalLight;
struct PointLight;
class DirectXCommon;

enum class LightType{
	Directional,
};

class LightManager final{
	struct LightCount{
        uint32_t dlCount;
        uint32_t plCount;
        uint32_t slCount;
	};

    std::weak_ptr<DirectXCommon> dxCommon_;

    Microsoft::WRL::ComPtr<ID3D12Resource> directionalResource_;
    DirectionalLight* directionalLight_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> pointResource_;
    PointLight* pointLight_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> spotResource_;
    SpotLight* spotLight_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> countResource_;
    LightCount* lightCount_ = nullptr;

	static LightManager* instance;
    static std::once_flag onceFlag_;

    const LightCount MAX_COUNT {20, 20, 20};

public:
	LightManager(const LightManager&) = delete;
    void operator=(const LightManager&) = delete;
    static LightManager* GetInstance();

	void Initialize(const std::weak_ptr<DirectXCommon>& dxCommon);
    void Update() const;
    void Draw() const;

private:
    LightManager() = default;
    ~LightManager() = default;
    static void Create();
    static void Finalize();
};

