#pragma once
#include "Object/Object.h"

class Triangle : public Object{
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
public:
	Triangle(DirectXCommon* dxCommon) : Object(dxCommon) {}
	void Initialize() override;
	void Update() override;
	void Draw() override;
};

