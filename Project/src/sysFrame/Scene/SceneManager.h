#pragma once

#include "Factory/AbstractSceneFactory.h"
class BaseScene;

class SceneManager{
    BaseScene* scene_ = nullptr;
    BaseScene* nextScene_ = nullptr;

    AbstractSceneFactory* factory_ = nullptr;

public:
	~SceneManager();
	void Update();
    void Draw() const;
    void ChangeScene(const std::string& scene);

    void SetFactory(AbstractSceneFactory* factory);

private:
    void SetNextScene(BaseScene* nextScene);
};

