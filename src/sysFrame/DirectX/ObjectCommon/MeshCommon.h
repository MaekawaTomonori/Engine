#pragma once
class DirectXCommon;

class MeshCommon{
    DirectXCommon* dxCommon_ = nullptr;

public:
	MeshCommon(DirectXCommon* dxCommon) :dxCommon_(dxCommon) {}

	void Initialize();
	void PreDraw() const;

	DirectXCommon* GetDXCommon() const;
};

