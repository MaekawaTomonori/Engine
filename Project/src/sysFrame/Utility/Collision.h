#pragma once
#include "AABB.h"
#include "Math/Vector3.h"

namespace Collision{
	bool IsCollision(const AABB& aabb, const Vector3& point);
};

