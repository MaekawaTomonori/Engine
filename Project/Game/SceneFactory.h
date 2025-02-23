#pragma once
#include <memory>

#include "Scene/AbstractSceneFactory.h"

class SceneFactory : public AbstractSceneFactory{
public:
    std::unique_ptr<BaseScene> Create(const std::string& name) override;
};

