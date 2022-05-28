#include "Scene.h"

using namespace Teller;

void Teller::SceneModule::Tick(float& deltaTime)
{
	ModuleCore::Tick(deltaTime);
	for (auto& e : agents_) {
		e->Tick();
	}
}

void Teller::SceneModule::AddAgent(std::shared_ptr<Agent>&& _agent)
{
	agents_.push_back(std::move(_agent));
}
