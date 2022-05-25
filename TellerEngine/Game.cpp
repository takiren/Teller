#include "Game.h"
using namespace Teller;

Teller::GameModule::GameModule()
{
	bUpdate = true;
}

void Teller::GameModule::SetEnable(bool enable)
{
	bEnabled = enable;
}
