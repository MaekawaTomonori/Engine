﻿#pragma once
#include <cmath>

#include "Matrix.h"
#include "Vector3.h"

struct Transform;

namespace MathUtils{
    const float F_PI = 3.14159265358979323846264338327950288f;
    const double PI = 3.14159265358979323846264338327950288;

    template<typename Type>
    Type Lerp(const Type& a, const Type& b, float t) {
        return a + (b - a) * t;
    }

    template<typename Type>
    Type Factorial(Type n) {
        if (n <= 0)return 1;

        return static_cast<Type>(n * Factorial(n - 1));
    }

    template <typename Type>
    Type Permutation(Type n, Type r) {
        return static_cast<Type>(Factorial(n) / Factorial(n - r));
    }

    namespace Matrix {
        Matrix3x3 MakeIdentity3x3();

        Matrix4x4 MakeIdentity();
        Matrix4x4 MakeTranslateMatrix(const Vector3& velocity);
        Matrix4x4 MakeScaleMatrix(const Vector3& scale);
        Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

        Matrix4x4 MakeRotateX(float rad);
        Matrix4x4 MakeRotateY(float rad);
        Matrix4x4 MakeRotateZ(float rad);
        Matrix4x4 MakeAffineMatrix(const ::Transform& transform);
        Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

        Matrix4x4 MakeOrthogonalMatrix(float left, float right, float top, float bottom, float znear, float zfar);
        Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
        Matrix4x4 MakeViewportMatrix(float width, float height, float top, float bottom, float depthMax, float depthMin);
    }
};

