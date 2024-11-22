#pragma once
#include <memory>

#include "DirectX/Pipeline/GraphicsPipeline.h"

class DirectXCommon;

class ParticleCommon{
    DirectXCommon* dxCommon_ = nullptr;
    std::unique_ptr<GraphicsPipeline> pipeline_;

public:
    ParticleCommon(DirectXCommon* dxCommon) :dxCommon_(dxCommon){}
    ~ParticleCommon() = default;
	void Initialize();
    void PreDraw() const;

    DirectXCommon* GetDXCommon() const {
        return dxCommon_;
    }
};

