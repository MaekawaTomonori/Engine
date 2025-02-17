#pragma once
#include "Object/Light/RawLight.h"
#include "System/Math/Vector3.h"
#include "System/Math/Vector4.h"

struct SpotLight{
	Vector4 color;

    Vector3 position;
    float intensity;

    Vector3 direction;
    float distance;

    float decay;
    float cosAngle;
    float falloffStart;
    float pad;
};

class RawSpotLight : public RawLight{
    SpotLight light_{};

public:
	RawSpotLight() = default;
	RawSpotLight(RawSpotLight&) = delete;
    RawSpotLight& operator=(RawSpotLight&) = delete;

    SpotLight& GetLight() {
        return light_;
    }

    void DefaultSetting() override;
    void Set(const std::string& uuid, const SpotLight& sl);
    void Save(std::string _path) override;

protected:
    void ImGuiSetting() override;
};
