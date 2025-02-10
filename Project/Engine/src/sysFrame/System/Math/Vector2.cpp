#include "Vector2.h"
#include "MathUtils.h"

Vector2 Vector2::Random() {
    return {
        MathUtils::Random(-1.f, 1.f),
        MathUtils::Random(-1.f, 1.f),
    };
}