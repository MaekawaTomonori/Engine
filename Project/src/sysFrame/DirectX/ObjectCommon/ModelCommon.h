#pragma once
#include <memory>

class DirectXCommon;
class GraphicsPipeline;

class ModelCommon{
    DirectXCommon* dxCommon_ = nullptr;

    std::shared_ptr<GraphicsPipeline> pipeline_;
public:
	ModelCommon(DirectXCommon* dxCommon) :dxCommon_(dxCommon) {}

    void Initialize();
    void PreDraw() const;

    DirectXCommon* GetDXCommon() const;
};

