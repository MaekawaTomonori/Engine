#pragma once
#include "DirectX/Lighting/LightManager.h"
#include "System/System.h"
#include "magic_enum.hpp"

class RawLight{

protected:
    std::string uuid_;
	LightType type_;
    bool enable_ = true;
public:
	RawLight();
	virtual ~RawLight() = default;

    RawLight(const RawLight&) = delete;
    RawLight& operator=(const RawLight&) = delete;

	virtual void DefaultSetting() = 0;

    void Update();

    bool IsEnable() const;

protected:
    virtual void ImGuiSetting() = 0;
};

inline RawLight::RawLight() {
    type_ = LightType::Directional;
    uuid_ = System::CreateUuid();
}

inline void RawLight::Update() {
#ifdef _DEBUG
    ImGuiSetting();
#endif
}

inline bool RawLight::IsEnable() const {
	return enable_;
}

