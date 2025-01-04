#pragma once
#include <memory>

#include "DirectX/Util/D3DResourceLeakChecker.h"
#include "Object/Camera/Camera.h"
#include "System/ImGui/ImGuiManager.h"
#include "System/Log/Log.h"

#include "EngineDebug.h"

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

	std::shared_ptr<ImGuiManager> imguiManager_;

    Log* logger_ = nullptr;
    TextureManager* textureManager_ = nullptr;
    ModelManager* modelManager_ = nullptr;
    SpriteCommon* spriteCommon_ = nullptr;
    ModelCommon* modelCommon_ = nullptr;

    static std::shared_ptr<Camera> defaultCamera_;

    const bool engineDebug_ = true;
    std::shared_ptr<EngineDebug> debugScene_;

public:
	void Initialize();
    void Update() const;
    void Draw() const;
    void EndFrame() const;
    void Finalize();

    bool IsActive() const;

    static Camera* GetDefaultCamera() {
        return defaultCamera_.get();
    }
};

