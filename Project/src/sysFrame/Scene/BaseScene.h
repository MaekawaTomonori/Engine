#pragma once
#include <string>

#include "Effect/SceneChanger.h"

class SceneManager;

class BaseScene{
	SceneManager* manager_ = nullptr;
    bool progress_ = false;
protected:
	std::string nextScene_;

    SceneChanger::Type entryType_ = SceneChanger::Type::NONE;
    SceneChanger::Type leaveType_ = SceneChanger::Type::NONE;

public:
	virtual ~BaseScene() = default;
	virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Finalize() = 0;
    void SetManager(SceneManager* manager);
    bool InProgress() const;
    void Awake();

    SceneChanger::Type GetEntryEffect() const;
    SceneChanger::Type GetLeaveEffect() const;

protected:
	void ChangeScene();
};

