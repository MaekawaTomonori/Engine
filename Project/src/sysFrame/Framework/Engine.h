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
#include "System/Log/Log.h"

class ParticleManager;

class Engine{
	std::unique_ptr<D3DResourceLeakChecker> leakChecker;
	std::unique_ptr<WinApp> winApp_;
    std::shared_ptr<DirectXCommon> dxCommon_;
    std::unique_ptr<SRVManager> srvManager_;
    std::unique_ptr<Input> input_;
	std::unique_ptr<ImGuiManager> imguiManager_;

    SpriteCommon* spriteCommon_;
    ModelCommon* modelCommon_;
    TextureManager* textureManager_;
    ModelManager* modelManager_;
    LightManager* lightManager_;
    ParticleManager* particle_;
    Audio* audio_;
    Log* log_;

	static std::unique_ptr<Camera> defaultCamera_;


public:
	Engine();
    void Initialize() const;
    void Update() const;
    void PreDraw() const;
    void PostDraw() const;
    void Finalize() const;

    bool IsEndRequest() const;
    static Camera* GetDefaultCamera();
};
