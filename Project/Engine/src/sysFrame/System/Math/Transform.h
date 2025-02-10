#pragma once
#include "Vector3.h"

struct Transform{
    Vector3 scale;
    Vector3 rotate;
    Vector3 translate;

    Transform operator+(const Transform& other) const {
        return {
            scale + other.scale,
            rotate + other.rotate,
            translate + other.translate
        };
    }
};

