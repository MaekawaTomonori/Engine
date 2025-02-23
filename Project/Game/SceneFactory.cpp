#include "SceneFactory.h"

#include "GameScene/GameScene.h"
#include "System/System.h"

std::unique_ptr<BaseScene> SceneFactory::Create(const std::string& name) {
    std::unique_ptr<BaseScene> scene;
    if (System::EqualsIgnoreCase(name, "Game") || System::EqualsIgnoreCase(name ,"Play")){
        scene = std::make_unique<GameScene>();
    }
    return scene;
}
