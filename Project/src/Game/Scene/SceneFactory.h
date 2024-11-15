#pragma once
#include "Scene/Factory/AbstractSceneFactory.h"

//Game側(ユーザー操作側)
class SceneFactory : public AbstractSceneFactory{
public:
	~SceneFactory() override = default;
	BaseScene* CreateScene(const std::string& scene) override;
};

