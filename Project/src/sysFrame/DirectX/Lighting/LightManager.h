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
    std::weak_ptr<DirectXCommon> dxCommon_;

    Microsoft::WRL::ComPtr<ID3D12Resource> directionalResource_;
    DirectionalLight* directionalLight_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> pointResource_;
    PointLight* pointLight_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> spotResource_;
    SpotLight* spotLight_ = nullptr;

    LightManager() = default;
    ~LightManager() = default;


	static LightManager* instance;
    static std::once_flag onceFlag_;

public:
	LightManager(const LightManager&) = delete;
    void operator=(const LightManager&) = delete;
    static LightManager* GetInstance();
    static void Create();
    static void Finalize();


	void Initialize(const std::weak_ptr<DirectXCommon>& dxCommon);
    void Update() const;
    void Draw() const;
};

