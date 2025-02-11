#pragma once
#include "Object/Light/RawLight.h"
#include "System/Math/Vector3.h"
#include "System/Math/Vector4.h"

struct DirectionalLight{
	Vector4 color;
	Vector3 direction;
    float intensity;
};

class RawDirectionalLight : public RawLight{
    DirectionalLight light_ {};

public:
    DirectionalLight& GetLight() {
        return light_;
    }

	void DefaultSetting() override;

protected:
    void ImGuiSetting() override;
};