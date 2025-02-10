#pragma once
#include <cmath>

#include "Vector2.h"

struct Vector3{
    float x, y, z;

    Vector3 operator+(const Vector3& v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

	void operator+=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}
	void operator+=(const Vector2& v) {
		x += v.x;
		y += v.y;
		z += 0;
	}

    Vector3 operator-(const Vector3& v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    Vector3 operator*(const float f) const {
        return {x * f, y * f, z * f};
    }

	Vector3 operator/(const float& f) const {
        return {x / f, y / f, z / f};
    }

	float length() const {
		return sqrtf(x * x + y * y + z * z);
	}

	Vector3 normalize() {
		this->x /= length();
		this->y /= length();
		this->z /= length();

		return *this;
	}

	static Vector3 Random();
};
