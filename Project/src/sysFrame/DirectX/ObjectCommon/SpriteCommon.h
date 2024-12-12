#pragma once
#include <memory>
#include <mutex>

class DirectXCommon;
class GraphicsPipeline;

class SpriteCommon{
    static SpriteCommon* instance_;
    static std::once_flag onceFlag_;

	DirectXCommon* dxCommon_ = nullptr;

    std::shared_ptr<GraphicsPipeline> pipeline_;

private://Methods
	SpriteCommon() = default;
    ~SpriteCommon() = default;

	void CreatePipeline();
public:
	SpriteCommon(const SpriteCommon&) = delete;
    void operator=(SpriteCommon&) = delete;

    static SpriteCommon* GetInstance();
    static void Create();
    static void Destroy();

	void Initialize(DirectXCommon* dxCommon);
    void PreDraw() const;

    DirectXCommon* GetDXCommon() const {
        return dxCommon_;
    }
};

