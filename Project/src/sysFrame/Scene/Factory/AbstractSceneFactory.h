#pragma once
#include <string>

#include "Scene/BaseScene.h"

//エンジン側
class AbstractSceneFactory{
public:
	virtual ~AbstractSceneFactory() = default;
    virtual BaseScene* CreateScene(const std::string& scene) = 0;
};

