#pragma once
#include "System/Math/Vector3.h"
#include "System/Math/Vector4.h"

struct DirectionalLight{
	Vector4 color;
	Vector3 direction;
    float intensity;
};
