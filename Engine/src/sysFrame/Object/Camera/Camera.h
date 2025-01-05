#pragma once
#include "System/Math/Vector3.h"
#include "System/Math/Transform.h"
#include "System/Math/Matrix.h"

struct CameraForGPU{
    Vector3 worldPosition;
};

class Camera{
    Transform transform_{};

    Matrix4x4 cameraMatrix {};
    Matrix4x4 viewMatrix {};
    Matrix4x4 projectionMatrix {};

    float fov_ = 0.45f;
    float aspectRatio_ = 0;

    float near_ = 0.1f;
    float farZ_ = 100.f;

public:
	Camera() = default;
	~Camera() = default;
	void Initialize();
    void Update();
    Matrix4x4 GetCameraMatrix() const;
	Matrix4x4 GetViewProjection() const;
    CameraForGPU GetCameraForGPU() const;
};

