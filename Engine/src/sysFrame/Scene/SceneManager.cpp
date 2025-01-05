#include "SceneManager.h"

#include "AbstractSceneFactory.h"
#include "BaseScene.h"
#include "System/System.h"

SceneManager::~SceneManager() {
    scene_.reset();
    next_.reset();
}

void SceneManager::Update() {
    if (next_){
        if (scene_){
            scene_->Finalize();
            scene_.reset();
            scene_ = nullptr;
        }

        scene_ = std::move(next_);
        next_.reset();
        next_ = nullptr;
        scene_->SetManager(this);
        scene_->Initialize();
    }
}

void SceneManager::Draw() const {
    if(!scene_)return;
    scene_->Draw();
}

//void SceneManager::SetNext(std::unique_ptr<BaseScene> next) {
//    next_ = std::move(next);
//}

void SceneManager::SetFactory(std::unique_ptr<AbstractSceneFactory> factory) {
    factory_ = std::move(factory);
}

void SceneManager::ChangeScene(const std::string& name) {
    if (!factory_){
        System::Log(Log::Level::ERR, "SceneFactory is not set");
        assert(false);
        return;
    }
    if (next_)return;

    next_ = factory_->Create(name);
}
