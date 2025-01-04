#pragma once
#include <memory>
class DirectXCommon;

class MeshCommon{
    std::weak_ptr<DirectXCommon> dxCommon_;

public:
	MeshCommon(const std::weak_ptr<DirectXCommon>& dxCommon) :dxCommon_(dxCommon) {}

	void Initialize();
	void PreDraw() const;

	std::weak_ptr<DirectXCommon> GetDXCommon() const;
};

