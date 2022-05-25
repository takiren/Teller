#include "Scene.h"

using namespace Teller;

void Teller::SceneModule::Tick()
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

void Teller::SceneModule::AddAgent(std::shared_ptr<Agent>&& _agent)
{
	agents_.push_back(_agent);
	_agent->parentScene = this->shared_from_this();
}
