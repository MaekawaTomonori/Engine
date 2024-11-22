#pragma once
#include "Utility/AABB.h"
#include "Utility/Math/Vector3.h"

struct AccelerationField{
    Vector3 acceleration;
    AABB area;
};

