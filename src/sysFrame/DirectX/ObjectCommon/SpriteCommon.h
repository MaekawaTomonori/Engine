#pragma once
#include <memory>

class DirectXCommon;
class GraphicsPipeline;

class SpriteCommon{
	DirectXCommon* dxCommon_ = nullptr;

    std::shared_ptr<GraphicsPipeline> pipeline_;

private://Methods
	void CreatePipeline();

public:
	SpriteCommon(DirectXCommon* dxCommon) : dxCommon_(dxCommon) {}
	void Initialize();
    void PreDraw() const;

    DirectXCommon* GetDXCommon() const {
        return dxCommon_;
    }
};

