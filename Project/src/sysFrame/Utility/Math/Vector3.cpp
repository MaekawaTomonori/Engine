#include "Vector3.h"

#include "MathUtils.h"

Vector3 Vector3::Random() {
    return {
        MathUtils::Random(-1.f, 1.f),
        MathUtils::Random(-1.f, 1.f),
        0
    };
}
