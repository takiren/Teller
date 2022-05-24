#include "Scene.h"

using namespace Teller;

void Teller::SceneModule::AddAgent(std::shared_ptr<Agent>&& _agent)
{
	SceneAgents.push_back(_agent);	
}
