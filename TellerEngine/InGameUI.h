#pragma once

#include"Game.h"
#include"Agent.h"

class InGameUIBase :public Agent{
private:
	Agent* Target();
public:
	InGameUIBase();
	~InGameUIBase();
};

class UIBase {
private:
	Agent* Target();
public:
	UIBase();
	~UIBase();
	bool bSetTargetAgent(Agent* target);
};