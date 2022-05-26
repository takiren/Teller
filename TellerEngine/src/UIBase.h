#pragma once
#include "Game.h"
#include "TellerEvent.h"

class UIBase {
private:
	std::unique_ptr<TellerEventBase> tevent;
public:
	UIBase() = default;
	virtual ~UIBase() = default;
	virtual void Interacted();
};

class UIButton :public UIBase{
private:
public:
	void Interacted();
	void Clicked();
};