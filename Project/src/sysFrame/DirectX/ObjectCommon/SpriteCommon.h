#pragma once
#include <memory>

class DirectXCommon;
class GraphicsPipeline;

class SpriteCommon{
    static std::shared_ptr<SpriteCommon> instance_;

	DirectXCommon* dxCommon_ = nullptr;

    std::shared_ptr<GraphicsPipeline> pipeline_;

private://Methods
	SpriteCommon() = default;
    ~SpriteCommon() = default;

	void CreatePipeline();
public:
	SpriteCommon(const SpriteCommon&) = delete;
    void operator=(SpriteCommon&) = delete;

    static std::shared_ptr<SpriteCommon> GetInstance();

	void Initialize(DirectXCommon* dxCommon);
    void PreDraw() const;

    DirectXCommon* GetDXCommon() const {
        return dxCommon_;
    }
};

