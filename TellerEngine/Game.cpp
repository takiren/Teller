#include "Game.h"
using namespace Teller;

Teller::GameModule::GameModule()
{
	bUpdate = false;
}

void Teller::GameModule::SetEnable(bool enable)
{
	bEnabled = enable;
}
