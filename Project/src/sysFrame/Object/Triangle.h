#pragma once
#include "Object/Object.h"
#include "Utility/Math/VertexData.h"

class Triangle : public Object{
	VertexData* vertexData_ = nullptr;
public:
	Triangle(const std::weak_ptr<DirectXCommon>& dxCommon) : Object(dxCommon) {}
	void Initialize() override;
	void Update() override;
	void Draw() override;
};

