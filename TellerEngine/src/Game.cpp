#include "Game.h"

using namespace teller;

void teller::GameModule::PushScene(std::shared_ptr<SceneModule> _scene)
{
	mScenes_.push(_scene);
}

void teller::GameModule::Tick(float& deltaTime)
{
	mScenes_.front()->Tick(deltaTime);
}

void teller::GameModule::SetEnable(bool enable)
{
	bEnabled = enable;
}

teller::Sequence::Sequence(std::string path)
{
	std::ifstream reading(path);
	reading >> jsfile;
}
