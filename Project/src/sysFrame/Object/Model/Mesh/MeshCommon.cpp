#include "MeshCommon.h"

void MeshCommon::Initialize() {
}

void MeshCommon::PreDraw() const {
}

std::weak_ptr<DirectXCommon> MeshCommon::GetDXCommon() const {
    return dxCommon_;
}
