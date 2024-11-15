#include "SceneManager.h"

#include "BaseScene.h"

SceneManager::~SceneManager() {
    delete nextScene_;
    delete scene_;
}

void SceneManager::Update() {
    if (!scene_ && !nextScene_)return;

    if(nextScene_){
        if (scene_){
            delete scene_;
            scene_ = nullptr;
        }

        scene_ = nextScene_;
        nextScene_ = nullptr;

        scene_->SetManager(this);
        scene_->Initialize();
    }

    scene_->Update();
}

void SceneManager::Draw() const {
    if (!scene_)return;

    scene_->Draw();
}

void SceneManager::SetNextScene(BaseScene* nextScene) {
    nextScene_ = nextScene;
}
