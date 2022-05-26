#include "Scene.h"

using namespace Teller;

void Teller::SceneModule::AddAgent(std::shared_ptr<Agent> _agent)
{
	agents_.push_back(_agent);
	_agent->parent = this->shared_from_this();
}
