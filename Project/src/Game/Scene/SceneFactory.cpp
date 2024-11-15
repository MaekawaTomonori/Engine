#include "SceneFactory.h"

#include "PlayScene.h"
#include "TitleScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& scene) {
    BaseScene* nextScene = nullptr;

    if (scene == "Title"){
        nextScene = new TitleScene();
    } else if (scene == "Play"){
        nextScene = new PlayScene();
    }else{
        assert(false);
    }

    return nextScene;
}
