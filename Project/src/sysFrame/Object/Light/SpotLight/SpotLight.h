#pragma once
#include "Utility/Math/Vector3.h"
#include "Utility/Math/Vector4.h"

struct SpotLight{
	Vector4 color;
    Vector3 position;
    float intensity;
    Vector3 direction;
    float distance;
    float decay;
    float cosAngle;
    float falloffStart;
    float pad;
};
