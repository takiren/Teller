#pragma once
#include "Game.h"
#include "cinder/Vector.h"

class Agent
{
private:
	cinder::vec3 position;
	cinder::vec3 scale;
	cinder::vec3 rotation;
	Agent* parent;
public:
	virtual ~Agent();
	virtual void Tick();
	virtual void Move();
	virtual void Scale();
	virtual void Rotate();
	virtual void MessageHandler();
};

