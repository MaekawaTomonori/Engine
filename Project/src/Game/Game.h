#pragma once
#include "Framework/Framework.h"

class Game : public Framework{
	
public:
	~Game() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
};

