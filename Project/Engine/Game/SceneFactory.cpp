#include "SceneFactory.h"

#include "GameScene.h"

std::unique_ptr<BaseScene> SceneFactory::Create(const std::string& name) {
    std::unique_ptr<BaseScene> scene;
    if (name == "Title"){
        //scene = std::make_unique<TitleScene>();
    }
    else if (name == "Game" || name == "Play"){
        scene = std::make_unique<GameScene>();
    }

    return scene;
}
