#pragma once
class SceneManager;

class BaseScene{
    SceneManager* manager_ = nullptr;
public:
	virtual ~BaseScene() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

    void SetManager(SceneManager* manager) {
        manager_ = manager;
    }
};

