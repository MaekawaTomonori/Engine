#pragma once
#include <memory>

#include "WindowsApplication/WinApp.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/Heap/SRVManager.h"
#include "DirectX/Lighting/LightManager.h"
#include "DirectX/Model/ModelManager.h"
#include "DirectX/ObjectCommon/ModelCommon.h"
#include "DirectX/ObjectCommon/SpriteCommon.h"
#include "DirectX/Texture/TextureManager.h"
#include "DirectX/Util/D3DResourceLeakChecker.h"
#include "System/ImGui/ImGuiManager.h"
#include "System/Input/Input.h"
#include "System/Sound/Audio.h"
#include "Object/Camera/Camera.h"

class Engine{
	std::unique_ptr<D3DResourceLeakChecker> leakChecker;
	std::unique_ptr<WinApp> winApp_;
    std::unique_ptr<DirectXCommon> dxCommon_;
    std::unique_ptr<SRVManager> srvManager_;
    std::unique_ptr<ImGuiManager> imguiManager_;
    std::shared_ptr<SpriteCommon> spriteCommon_;
    std::shared_ptr<ModelCommon> modelCommon_;
    std::shared_ptr<TextureManager> textureManager_;
    std::shared_ptr<ModelManager> modelManager_;
    std::shared_ptr<LightManager> lightManager_;
    std::unique_ptr<Input> input_;
    std::shared_ptr<Audio> audio_;
	static std::unique_ptr<Camera> defaultCamera_;

public:
	Engine();
    void Initialize() const;
    void Update() const;
    void PreDraw() const;
    void PostDraw() const;

    bool IsEndRequest() const;
    static Camera* GetDefaultCamera();

    DirectXCommon* GetDirectXCommon() const;
    SpriteCommon* GetSpriteCommon() const;
    ModelCommon* GetModelCommon() const;
};
