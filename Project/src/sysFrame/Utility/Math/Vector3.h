#pragma once
#include <cmath>

struct Vector3{
    float x, y, z;

	void operator+=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}

	float length() const {
		return sqrtf(x * x + y * y + z * z);
	}

	void normalize() {
		this->x /= length();
		this->y /= length();
		this->z /= length();
	}
};

