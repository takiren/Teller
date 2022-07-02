#include "Game.h"

using namespace teller;

void teller::GameModule::PushScene(std::shared_ptr<SceneModule> _scene)
{
	mScenes_.push(_scene);
}

void teller::GameModule::Tick(float& deltaTime)
{
	mScenes_.top()->Tick(deltaTime);
}


teller::Sequence::Sequence(std::string path)
{
	std::ifstream reading(path);
	reading >> jsfile;
}
