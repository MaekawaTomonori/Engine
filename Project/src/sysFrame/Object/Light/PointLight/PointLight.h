#pragma once
#include "Utility/Math/Vector3.h"
#include "Utility/Math/Vector4.h"

struct PointLight{
	Vector4 color;
	Vector3 position;
	float intensity;
	float radius;
	float decay;
	float pad[2];
};
