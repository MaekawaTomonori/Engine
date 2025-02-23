#pragma once
#include <memory>
#include <string>

#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include "Effect/SceneChanger.h"

class SceneManager{
    std::unique_ptr<AbstractSceneFactory> factory_;

    std::unique_ptr<BaseScene> scene_;
    std::unique_ptr<BaseScene> next_;

    std::unique_ptr<SceneChanger> changer_;

public:
    SceneManager();
	~SceneManager();
	void Update();
    void Draw() const;

    void SetFactory(std::unique_ptr<AbstractSceneFactory> factory);

    void ChangeScene(const std::string& name);
};

