#pragma once
#include <memory>

#include "DirectX/Util/D3DResourceLeakChecker.h"
#include "Object/Camera/Camera.h"
#include "System/ImGui/ImGuiManager.h"

class TextureManager;

class Engine{

	std::shared_ptr<D3DResourceLeakChecker> leakChecker = std::make_shared<D3DResourceLeakChecker>();

    std::shared_ptr<WinApp> winApp_ ;
    std::shared_ptr<DirectXCommon> dxCommon_;

	std::shared_ptr<ImGuiManager> imguiManager_;

    TextureManager* textureManager_ = nullptr;

    static std::shared_ptr<Camera> defaultCamera_;

    bool engineDebug_ = false;

public:
	void Initialize();
    void Update() const;
    void Draw() const;
    void EndFrame() const;
    void Finalize();

    bool IsActive() const;
};

