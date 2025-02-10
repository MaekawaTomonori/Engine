#include "Camera.h"

#include "Application/WinApp.h"
#include "imgui/imgui.h"
#include "System/Math/MathUtils.h"

Camera::Camera() {
    UUID uuid;
    UuidCreate(&uuid);
    RPC_CSTR szUuid = nullptr;
    UuidToStringA(&uuid, &szUuid);
    uuid_ = reinterpret_cast<char*>(szUuid);
    RpcStringFreeA(&szUuid);
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
#ifdef _DEBUG
    ImGui::Begin("Camera");
    if(ImGui::TreeNode(uuid_.c_str())) {
        ImGui::DragFloat3("Pos", &transform_.translate.x, 0.01f);
        ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);
        ImGui::TreePop();
    }
    ImGui::End();
#endif

    cameraMatrix = MathUtils::Matrix::MakeAffineMatrix(transform_);
	viewMatrix = cameraMatrix.Inverse();
	projectionMatrix = MathUtils::Matrix::MakePerspectiveFovMatrix(fov_, aspectRatio_, near_, farZ_);
}

Camera* Camera::SetRotate(Vector3 rotation) {
    transform_.rotate = rotation;
    return this;
}

Camera* Camera::SetPosition(Vector3 position) {
    transform_.translate = position;
    return this;
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
