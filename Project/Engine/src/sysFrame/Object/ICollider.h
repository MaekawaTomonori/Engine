#pragma once
#include "System/Math/AABB.h"
#include "System/Math/Vector2.h"
#include "System/Math/Vector3.h"

class ICollider {
protected:
	Vector3 center{};
};

class SphereCollider : public ICollider {
protected:
	Vector3 radius{};
};

class BoxCollider : public ICollider {
protected:
	Vector3 size{};

public:
	AABB GetAABB() const {
		return { center - size / 2.f, center + size / 2.f };
	}
};

class ICollider2d {
protected:
	Vector2 center{};

public:
	virtual ~ICollider2d() = default;

	virtual void OnCollision() = 0;
};

class SphereCollider2d : ICollider2d {
protected:
	Vector2 radius{};
};

class BoxCollider2d : public ICollider2d {
protected:
	Vector2 size{};
};
