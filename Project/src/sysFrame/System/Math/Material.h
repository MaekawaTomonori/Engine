#pragma once
#include "Vector4.h"

struct Material{
	Vector4 color;

	/**
     * 0 : NoLight
	 * 1 : LambertianReflection
     * 2 : HalfLambertianReflection
	 */
	uint32_t enableLight;
	//float padding[3];
	//Matrix4x4 uvTranform;
};
