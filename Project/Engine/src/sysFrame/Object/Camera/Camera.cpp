#include "Camera.h"

#include "Application/WinApp.h"
#include "imgui/imgui.h"
#include "System/System.h"
#include "System/Math/MathUtils.h"

Camera::Camera() {
    uuid_ = System::CreateUuid();
}

void Camera::Initialize() {
    transform_ = {
        {1,1,1},
        {0,0,0},
        {0,0,-5}
    };
    aspectRatio_ = static_cast<float>(WinApp::CLIENT_WIDTH) / static_cast<float>(WinApp::CLIENT_HEIGHT);
}

void Camera::Update() {
    cameraMatrix = MathUtils::Matrix::MakeAffineMatrix(transform_);
	viewMatrix = cameraMatrix.Inverse();
	projectionMatrix = MathUtils::Matrix::MakePerspectiveFovMatrix(fov_, aspectRatio_, near_, farZ_);
}

void Camera::ImGui() {
#ifdef _DEBUG
    if(ImGui::TreeNode(uuid_.c_str())) {
        ImGui::DragFloat3("Pos", &transform_.translate.x, 0.01f);
        ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);
        ImGui::TreePop();
    }
#endif
}

Matrix4x4 Camera::GetCameraMatrix() const {
    return cameraMatrix;
}

Matrix4x4 Camera::GetViewProjection() const {
    return viewMatrix * projectionMatrix;
}

CameraForGPU Camera::GetCameraForGPU() const {
    return {transform_.translate};
}
