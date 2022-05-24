#pragma once

#include"Game.h"
#include"Agent.h"

class InGameUIBase :public Agent{
private:
	Agent* Target();
public:
	InGameUIBase();
	~InGameUIBase();

	virtual void Draw();
};

template<class T>
class UITemplate {
	std::unique_ptr<T> ui;
public:
	UITemplate(std::unique_ptr<T>&& target) :
		ui(std::move(target)) {};

};

class UIBase {
private:
	Agent* Target();
public:
	UIBase();
	~UIBase();
	bool bSetTargetAgent(Agent* target);
};

class MainTextBox :public InGameUIBase {
private:
	int x, y, sizeX_, sizeY_;
public:
	MainTextBox(int screenX, int screenY, int sizeX, int sizeY) :
		x(screenX),
		y(screenY),
		sizeX_(sizeX),
		sizeY_(sizeY) {};
	~MainTextBox() = default;

};