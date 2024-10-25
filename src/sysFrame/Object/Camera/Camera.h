#pragma once
#include "System/Math/Matrix.h"
#include "System/Math/Transform.h"

class Camera{
    Transform transform_{};

    float fov_ = 0.45f;

    float near_ = 0.1f;
    float farZ_ = 100.f;

public:
	void Initialize();
	Matrix4x4 GetViewProjection() const;
};

