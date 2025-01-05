#pragma once
#include <memory>
#include <mutex>

class DirectXCommon;
class GraphicsPipeline;

class SpriteCommon{
    static SpriteCommon* instance_;
    static std::once_flag onceFlag_;

	std::weak_ptr<DirectXCommon> dxCommon_;

    std::shared_ptr<GraphicsPipeline> pipeline_;

private://Methods
	SpriteCommon() = default;
    ~SpriteCommon() = default;

	void CreatePipeline();

    static void Create();
    static void Destroy();
public:
	SpriteCommon(const SpriteCommon&) = delete;
    void operator=(SpriteCommon&) = delete;

    static SpriteCommon* GetInstance();

	void Initialize(const std::weak_ptr<DirectXCommon>& dxCommon);
    void PreDraw() const;

    std::weak_ptr<DirectXCommon> GetDXCommon() const {
        return dxCommon_;
    }
};

