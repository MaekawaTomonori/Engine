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
}

void Camera::Update() {
    ImGui::Begin("Camera");
    ImGui::DragFloat3("Pos : ", &transform_.translate.x, 0.01f);
    ImGui::End();
}

Matrix4x4 Camera::GetViewProjection() const {
    Matrix4x4 view = MathUtils::Matrix::MakeAffineMatrix(transform_).Inverse();
    Matrix4x4 projection = MathUtils::Matrix::MakePerspectiveFovMatrix(fov_, static_cast<float>(WinApp::CLIENT_WIDTH) / static_cast<float>(WinApp::CLIENT_HEIGHT) , near_, farZ_);
    return view * projection;
}
