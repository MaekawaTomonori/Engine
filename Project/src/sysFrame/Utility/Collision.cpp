#include "Collision.h"

bool Collision::IsCollision(const AABB& aabb, const Vector3& point) {
    return (aabb.min.x <= point.x && point.x <= aabb.max.x) &&
        (aabb.min.y <= point.y && point.y <= aabb.max.y) &&
        (aabb.min.z <= point.z && point.z <= aabb.max.z);
}
