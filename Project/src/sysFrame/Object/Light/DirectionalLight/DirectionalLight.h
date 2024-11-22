#pragma once
#include "Utility/Math/Vector3.h"
#include "Utility/Math/Vector4.h"

struct DirectionalLight{
	Vector4 color;
	Vector3 direction;
    float intensity;
};
