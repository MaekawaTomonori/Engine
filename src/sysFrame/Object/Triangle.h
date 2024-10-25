#pragma once
#include "Object/Object.h"

class Triangle : public Object{
public:
	Triangle(DirectXCommon* dxCommon) : Object(dxCommon) {}
	void Initialize() override;
	void Update() override;
	void Draw() override;
};

