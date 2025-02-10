#pragma once
#include <memory>
#include <mutex>
#include <vector>

enum class BlendMode;
class DirectXCommon;
class GraphicsPipeline;

class SpriteCommon{
    static SpriteCommon* instance_;
    static std::once_flag onceFlag_;

	std::weak_ptr<DirectXCommon> dxCommon_;

    std::vector<std::unique_ptr<GraphicsPipeline>> pipelines_;

    BlendMode mode_{};

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

    void SetBlendMode(BlendMode mode);

    std::weak_ptr<DirectXCommon> GetDXCommon() const {
        return dxCommon_;
    }
};

