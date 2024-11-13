#include "Camera.h"

#include "WindowsApplication/WinApp.h"
#include "imgui/imgui.h"
#include "System/Math/MathUtils.h"

void Camera::Initialize() {
    transform_ = {
        {1,1,1},
        {0,0,0},
        {0,0,-5}
    };
    aspectRatio_ = static_cast<float>(WinApp::CLIENT_WIDTH) / static_cast<float>(WinApp::CLIENT_HEIGHT);
}

void Camera::Update() {
#ifdef _DEBUG
    ImGui::Begin("Camera");
    ImGui::DragFloat3("Pos : ", &transform_.translate.x, 0.01f);
    ImGui::End();
#endif

	viewMatrix = MathUtils::Matrix::MakeAffineMatrix(transform_).Inverse();
	projectionMatrix = MathUtils::Matrix::MakePerspectiveFovMatrix(fov_, aspectRatio_, near_, farZ_);
}

Matrix4x4 Camera::GetViewProjection() const {
    return viewMatrix * projectionMatrix;
}
