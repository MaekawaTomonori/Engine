#include "SceneFactory.h"

#include "PlayScene.h"
#include "TitleScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
    BaseScene* scene = nullptr;

    if (sceneName == "Title"){
        scene = new TitleScene();
    } else if (sceneName == "Play"){
        scene = new PlayScene();
    }else{
        assert(false);
    }

    return scene;
}
