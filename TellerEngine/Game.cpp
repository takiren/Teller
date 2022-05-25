#include "Game.h"
#include<iostream>
using namespace Teller;

void Teller::GameModule::Tick()
{
	count_++;
	if (bUpdate) {
		for (auto& e : children) {
			e.get()->Tick();
			e.get()->Update();
		}
	}
	else
	{
		for (auto& e : children) {
			e.get()->Tick();
		}
	}
}

void Teller::GameModule::SetEnable(bool enable)
{
	bEnabled = enable;
}
