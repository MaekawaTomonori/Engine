#include "ISceneEffect.h"

void ISceneEffect::Initialize() {
    if (!fInit_)return;
    (this->*fInit_)();
}

bool ISceneEffect::Update() {
    if (!fUpdate_)return false;

    return (this->*fUpdate_)();
}

void ISceneEffect::SetMode(State mode) {
    if (mode == State::In){
        fInit_ = &ISceneEffect::InitIn;
        fUpdate_ = &ISceneEffect::UpdateIn;
    } else{
        fInit_ = &ISceneEffect::InitOut;
        fUpdate_ = &ISceneEffect::UpdateOut;
    }
}

