#include "Camera.h"

#include "Application/WinApp.h"
#include "System/Math/MathUtils.h"

void Camera::Initialize() {
    transform_ = {
        {1,1,1},
        {0,0,0},
        {0,0,-5}
    };
}

Matrix4x4 Camera::GetViewProjection() const {
    Matrix4x4 view = MathUtils::Matrix::MakeAffineMatrix(transform_).Inverse();
    Matrix4x4 projection = MathUtils::Matrix::MakePerspectiveFovMatrix(fov_, static_cast<float>(WinApp::CLIENT_WIDTH / WinApp::CLIENT_HEIGHT) , near_, farZ_);
    return view * projection;
}
