#include "WorldTransform.h"

#include "MathUtils.h"
#include "DirectX/DirectXCommon.h"
#include "Framework/Engine.h"

WorldTransform::WorldTransform(DirectXCommon* direx) {
    dxCommon_ = direx;
}

void WorldTransform::Initialize() {
    scale = {1,1,1};
    rotate = {};
    translate = {};

    resource_.Attach(DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(TransformationMatrix)));

    resource_->Map(0, nullptr, reinterpret_cast<void**>(&matrix_));

    matrix_->wvp = MathUtils::Matrix::MakeIdentity();

    camera_ = Engine::GetDefaultCamera();
}

void WorldTransform::Update() const {
    if (!camera_){
        matrix_->world = matrix_->wvp = MathUtils::Matrix::MakeAffineMatrix(scale, rotate, translate);
        return;
    }

    Matrix4x4 viewProjection = camera_->GetViewProjection();
    matrix_->world = MathUtils::Matrix::MakeAffineMatrix(scale, rotate, translate);
    matrix_->wvp = matrix_->world * viewProjection;
}
