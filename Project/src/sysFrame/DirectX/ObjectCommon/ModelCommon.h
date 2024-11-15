#pragma once
#include <memory>

class DirectXCommon;
class GraphicsPipeline;

class ModelCommon{

	DirectXCommon* dxCommon_ = nullptr;

    std::shared_ptr<GraphicsPipeline> pipeline_;

private:
    static std::shared_ptr<ModelCommon> instance_;
	ModelCommon();
    ~ModelCommon();

public:
	ModelCommon(const ModelCommon&) = delete;
    ModelCommon& operator=(const ModelCommon&) = delete;

    static std::shared_ptr<ModelCommon> GetInstance();

    void Initialize(DirectXCommon* dxCommon);
    void PreDraw() const;

    DirectXCommon* GetDXCommon() const;
};

