#pragma once
#include <cmath>

struct Vector3{
    float x, y, z;

	void operator+=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}

    Vector3 operator*(const float f) const {
        return {x * f, y * f, z * f};
    }

	float length() const {
		return sqrtf(x * x + y * y + z * z);
	}

	void normalize() {
		this->x /= length();
		this->y /= length();
		this->z /= length();
	}

	static Vector3 Random();
};
