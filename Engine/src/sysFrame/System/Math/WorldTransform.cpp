#include "WorldTransform.h"

#include "MathUtils.h"
#include "DirectX/DirectXCommon.h"
#include "Engine/Engine.h"

WorldTransform::WorldTransform(const std::weak_ptr<DirectXCommon>& direx) {
    dxCommon_ = direx;
}

void WorldTransform::Initialize(Type type) {
    scale = {1,1,1};
    rotate = {};
    translate = {};

    type_ = type;

    size_t size = type==Type::MODEL ? sizeof(TransformationMatrixForModel) : sizeof(TransformationMatrix);

	resource_.Attach(DirectXCommon::CreateBufferResource(dxCommon_.lock()->GetDevice(), size).Get());

    switch (type){
    case Type::SPRITE:
        resource_->Map(0, nullptr, reinterpret_cast<void**>(&matrix_));

        matrix_->wvp = MathUtils::Matrix::MakeIdentity();
        break;

    case Type::MODEL:
        resource_->Map(0, nullptr, reinterpret_cast<void**>(&matrixForModel_));

        matrixForModel_->world = MathUtils::Matrix::MakeIdentity();
        break;
    }
    camera_ = Engine::GetDefaultCamera();
}

void WorldTransform::Update() const {
    if (type_ == Type::SPRITE){
        matrix_->world = matrix_->wvp = MathUtils::Matrix::MakeAffineMatrix(scale, rotate, translate);
        return;
    }

    Matrix4x4 viewProjection = camera_->GetViewProjection();
    
    matrixForModel_->world = MathUtils::Matrix::MakeAffineMatrix(scale, rotate, translate);
    matrixForModel_->wvp = matrixForModel_->world * viewProjection;
    matrixForModel_->worldInverseTranspose = matrixForModel_->world.Inverse().Transpose();
}
