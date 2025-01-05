#pragma once
#include <memory>
#include <mutex>

class DirectXCommon;
class GraphicsPipeline;

class ModelCommon{

	std::weak_ptr<DirectXCommon> dxCommon_;

    std::shared_ptr<GraphicsPipeline> pipeline_;

private:
    static ModelCommon* instance_;
    static std::once_flag onceFlag_;
	ModelCommon() = default;
    ~ModelCommon() = default;

public:
	ModelCommon(const ModelCommon&) = delete;
    ModelCommon& operator=(const ModelCommon&) = delete;

	static ModelCommon* GetInstance();
    static void Create();
	static void Finalize();

    void Initialize(const std::weak_ptr<DirectXCommon>& dxCommon);
    void PreDraw() const;

    std::weak_ptr<DirectXCommon> GetDXCommon() const;
};

