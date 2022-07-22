#include "Game.h"

using namespace teller;

void teller::GameModule::PushScene(std::shared_ptr<SceneModule> _scene)
{
	mScenes_.push(_scene);
}

void teller::GameModule::Update(float& deltaTime)
{
	mScenes_.top()->Update(deltaTime);
}


void teller::GameModule::Draw()
{
	mScenes_.top()->Draw();
}


teller::Sequence::Sequence(std::string path)
{
	std::ifstream reading(path);
	reading >> jsfile;
}
