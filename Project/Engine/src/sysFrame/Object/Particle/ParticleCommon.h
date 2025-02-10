#pragma once
#include "DirectX/DirectXCommon.h"
#include "DirectX/Pipeline/GraphicsPipeline.h"

class ParticleCommon{
    DirectXCommon* dxCommon_ = nullptr;

    std::unique_ptr<GraphicsPipeline> pipeline_;

public:
	ParticleCommon(DirectXCommon* dxCommon) : dxCommon_(dxCommon) {}

    void Initialize();
    void Draw() const;

    DirectXCommon* GetDXCommon() const {
        return dxCommon_;
    }
};

