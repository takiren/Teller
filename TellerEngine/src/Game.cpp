#include "Game.h"

using namespace Teller;

void Teller::GameModule::PushScene(std::shared_ptr<SceneModule> _scene)
{
	mScenes_.push(_scene);
}

void Teller::GameModule::Tick(float& deltaTime)
{
	mScenes_.front()->Tick(deltaTime);
}

void Teller::GameModule::SetEnable(bool enable)
{
	bEnabled = enable;
}

Teller::Sequence::Sequence(std::string path)
{
	std::ifstream reading(path);
	reading >> jsfile;
}
