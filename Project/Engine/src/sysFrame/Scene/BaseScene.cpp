#include "BaseScene.h"
#include "Scene/SceneManager.h"

void BaseScene::SetManager(SceneManager* manager) {
	manager_ = manager;
}

bool BaseScene::InProgress() const {
    return progress_;
}

void BaseScene::Awake() {
    progress_ = true;
}

SceneChanger::Type BaseScene::GetEntryEffect() const {
    return entryType_;
}

SceneChanger::Type BaseScene::GetLeaveEffect() const {
    return leaveType_;
}

void BaseScene::ChangeScene() {
    progress_ = false;
    manager_->ChangeScene(nextScene_);
}

