#pragma once
#include <memory>
#include <string>

class BaseScene;

class AbstractSceneFactory{
public:
    virtual ~AbstractSceneFactory() = default;
    virtual std::unique_ptr<BaseScene> Create(const std::string& name) = 0;
};

