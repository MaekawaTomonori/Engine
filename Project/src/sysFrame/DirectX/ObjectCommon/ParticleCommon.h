#pragma once
#include <memory>

#include "DirectX/Pipeline/GraphicsPipeline.h"

class DirectXCommon;

class ParticleCommon{
    std::weak_ptr<DirectXCommon> dxCommon_;
    std::unique_ptr<GraphicsPipeline> pipeline_;

public:
    ParticleCommon(const std::weak_ptr<DirectXCommon>& dxCommon) :dxCommon_(dxCommon){}
    ~ParticleCommon();
	void Initialize();
    void PreDraw() const;

    std::weak_ptr<DirectXCommon> GetDXCommon() const {
        return dxCommon_;
    }
};

