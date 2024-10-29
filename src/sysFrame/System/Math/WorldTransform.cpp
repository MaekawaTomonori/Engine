#include "WorldTransform.h"

#include "MathUtils.h"
#include "DirectX/DirectXCommon.h"

WorldTransform::WorldTransform(DirectXCommon* direx) {
    dxCommon_ = direx;
}

void WorldTransform::Initialize() {
    transform_ = {};

    resource_.Attach(DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(TransformationMatrix)));

    resource_->Map(0, nullptr, reinterpret_cast<void**>(&matrix_));

    matrix_->wvp = MathUtils::Matrix::MakeIdentity();
}

void WorldTransform::Update() {
    if (!camera_){
        matrix_->wvp = MathUtils::Matrix::MakeAffineMatrix(transform_);
        return;
    }

    Matrix4x4 viewProjection = camera_->GetViewProjection();
    matrix_->wvp = MathUtils::Matrix::MakeAffineMatrix(transform_) * viewProjection;
}
