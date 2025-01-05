#pragma once
#include <memory>
#include <string>

#include "AbstractSceneFactory.h"
#include "BaseScene.h"

class SceneManager{
    std::unique_ptr<AbstractSceneFactory> factory_;

    std::unique_ptr<BaseScene> scene_;
    std::unique_ptr<BaseScene> next_;

public:
	~SceneManager();
	void Update();
    void Draw() const;

	//void SetNext(std::unique_ptr<BaseScene> next);
    void SetFactory(std::unique_ptr<AbstractSceneFactory> factory);

    void ChangeScene(const std::string& name);
};

