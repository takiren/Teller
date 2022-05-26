#include "Game.h"

using namespace Teller;

void Teller::GameModule::SetEnable(bool enable)
{
	bEnabled = enable;
}

Teller::Sequence::Sequence(std::string path)
{
	std::ifstream reading(path);
	reading >> jsfile;
}
