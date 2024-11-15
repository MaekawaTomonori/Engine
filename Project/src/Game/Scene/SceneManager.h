#pragma once
class BaseScene;

class SceneManager{
    BaseScene* scene_ = nullptr;
    BaseScene* nextScene_ = nullptr;

public:
	~SceneManager();
	void Update();
    void Draw() const;
    void SetNextScene(BaseScene* nextScene);
};

