#pragma once
#include "Object/Light/RawLight.h"
#include "System/Math/Vector3.h"
#include "System/Math/Vector4.h"

struct PointLight{
	Vector4 color;

	Vector3 position;
	float intensity;

	float radius;
	float decay;
    float pad[2];
};

class RawPointLight final : public RawLight{
	PointLight light_ {};
public:
    PointLight& GetLight() {
        return light_;
    }

	void DefaultSetting() override;
	void Set(const PointLight& pl);

protected:
	void ImGuiSetting() override;
};

