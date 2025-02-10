#include "SceneChanger.h"

#include "ISceneEffect.h"
#include "Fade.h"

void SceneChanger::Set(Type type, ISceneEffect::State state) {
    mode_ = state;
    Apply(type);

    if (!effect_) return;
    effect_->SetMode(mode_);
    effect_->Initialize();
}

void SceneChanger::Awake(Type type, ISceneEffect::State state) {
    isInProgress_ = true;
    Set(type, state);
}

void SceneChanger::Update() {
    if (!isInProgress_)return;
    if (!effect_){
        isInProgress_ = false;
        return;
    }

    if (!effect_->Update()){
        isInProgress_ = false;
    }
}

void SceneChanger::Draw() const {
    if (!effect_) return;

    effect_->Draw();
}

bool SceneChanger::InProgress() const {
    return isInProgress_;
}

void SceneChanger::Apply(Type type) {
    if (effect_){
        effect_.reset();
        effect_ = nullptr;
    }

    switch (type){
    case Type::NONE:
	    break;
    case Type::FADE:
        effect_ = std::make_unique<Fade>();
	    break;
    case Type::SLIDE:
	    break;
    }
}
