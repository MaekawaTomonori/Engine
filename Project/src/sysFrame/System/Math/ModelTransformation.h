#pragma once
#include "Matrix.h"

struct TransformationMatrix{
    Matrix4x4 wvp;
    Matrix4x4 world;
};

struct TransformationMatrixForModel : TransformationMatrix{
    Matrix4x4 worldInverseTranspose;
};
