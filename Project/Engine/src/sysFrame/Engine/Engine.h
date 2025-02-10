#pragma once
#include <memory>

#include "DirectX/Util/D3DResourceLeakChecker.h"
#include "Object/Camera/Camera.h"
#include "System/ImGui/ImGuiManager.h"
#include "System/Log/Log.h"

#include "EngineDebug.h"
#include "Object/Particle/ParticleManager.h"
#include "System/Input/Input.h"

#include "../AudioAnther.h"

class LightManager;
class SRVManager;
class TextureManager;
class ModelManager;
class SpriteCommon;
class ModelCommon;

class Engine{

	std::shared_ptr<D3DResourceLeakChecker> leakChecker = std::make_shared<D3DResourceLeakChecker>();

    std::shared_ptr<WinApp> winApp_ ;
    std::shared_ptr<DirectXCommon> dxCommon_;

    std::shared_ptr<SRVManager> srvManager_;


    Log* logger_ = nullptr;
    AudioAnther* audio_ = nullptr;
    Input* input_ = nullptr;

    ImGuiManager* imguiManager_ = nullptr;
    TextureManager* textureManager_ = nullptr;
    ModelManager* modelManager_ = nullptr;
    SpriteCommon* spriteCommon_ = nullptr;
    ModelCommon* modelCommon_ = nullptr;
    LightManager* light_ = nullptr;

    ParticleManager* particleManager_ = nullptr;

    static std::shared_ptr<Camera> defaultCamera_;

    bool engineDebug_ = false;
    std::shared_ptr<EngineDebug> debugScene_;

    std::string title_ = "Engine";
public:
	void Initialize();
    void Update() const;
    void Draw() const;
    void EndFrame() const;
    void Finalize() const;

    bool IsActive() const;

    void EnableDebug();
    bool IsDebug() const;

    void SetBackColor(const Vector4& color) const;

    void SetTitle(const std::string& title);

    void SetFPSLimit(const int fps) const {
        dxCommon_->SetFPSLimit(fps);
    }

	static Camera* GetDefaultCamera() {
        return defaultCamera_.get();
    }
};

