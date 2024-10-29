#pragma once
#include <memory>
class GraphicsPipeline;
class DirectXCommon;

class SpriteBase{
	DirectXCommon* dxCommon_ = nullptr;

    std::shared_ptr<GraphicsPipeline> pipeline_;

private://Methods
	void CreatePipeline();

public:
	SpriteBase(DirectXCommon* dxCommon) : dxCommon_(dxCommon) {}
	void Initialize();
    void PreDraw() const;

    DirectXCommon* GetDXCommon() const {
        return dxCommon_;
    }
};

