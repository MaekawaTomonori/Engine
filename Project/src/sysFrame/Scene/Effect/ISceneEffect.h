#pragma once

class ISceneEffect{
public:
	enum class State{
	    In,
	    Out
	};

private:
	void (ISceneEffect::*fInit_)() = nullptr;
	bool (ISceneEffect::*fUpdate_)() = nullptr;

protected:
	/// <summary>
	/// In Methods
	/// </summary>
	virtual void InitIn() = 0;
	virtual bool UpdateIn() = 0;

    /// <summary>
    /// Out Methods
    /// </summary>
	virtual void InitOut() = 0;
	virtual bool UpdateOut() = 0;
public:
	virtual ~ISceneEffect() = default;
	void Initialize();
	bool Update();
	virtual void Draw() = 0;

	void SetMode(State mode);
};

