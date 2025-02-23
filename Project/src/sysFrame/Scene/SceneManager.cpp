#include "SceneManager.h"

#include "System/System.h"

SceneManager::SceneManager() {
    changer_ = std::make_unique<SceneChanger>();
}

SceneManager::~SceneManager() {
    scene_.reset();
    next_.reset();
}

void SceneManager::Update() {
    // Scene Changer
	if (changer_->InProgress()){
	    changer_->Update();
	    return;
	}

    //　次のシーンが入った時
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
        scene_->Update();

        changer_->Awake(scene_->GetEntryEffect(), ISceneEffect::State::In);
    }

    if (!scene_)return;

    // ChangeProcess End
    if(changer_->InProgress())return;

    if (!scene_->InProgress()){
    	scene_->Awake();
    }

    scene_->Update();
}

void SceneManager::Draw() const {
    if (scene_){
        scene_->Draw();
    }

    changer_->Draw();
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

    if(!scene_) return;
    changer_->Awake(scene_->GetLeaveEffect(), ISceneEffect::State::Out);
}
