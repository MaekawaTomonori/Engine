#pragma once
#include "Vector4.h"

struct Material{
	Vector4 color;
	uint32_t enableLight;
	//float padding[3];
	//Matrix4x4 uvTranform;
};
