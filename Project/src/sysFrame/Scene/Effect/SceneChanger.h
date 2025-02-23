#pragma once
#include <memory>

#include "ISceneEffect.h"

class SceneChanger{
public:
	enum class Type{
		NONE,
		FADE,
	    SLIDE,
	};


private:
	std::unique_ptr<ISceneEffect> effect_;
	bool isInProgress_ = false;
	ISceneEffect::State mode_ = ISceneEffect::State::Out;

public:
	void Awake(Type type, ISceneEffect::State state);

	void Update();
	void Draw() const;

	bool InProgress() const;
private:
	void Set(Type type, ISceneEffect::State state);
	void Apply(Type type);
};
