#include "Scene.h"

using namespace teller;

void teller::SceneModule::Update(float& deltaTime)
{
	for (auto& agent : tAgents_) {
		agent->Update();
	}
}

void teller::SceneModule::Draw()
{
	gl::clear(Color::black());
	gl::color(Color::white());

	for (auto& agent : tAgents_)
		agent->Draw();

	for (auto& e : agents_)
		e->Update();
}

void teller::SceneModule::AddAgent(AgentRef _agent)
{
	agents_.push_back(_agent);
}

void teller::SceneModule::SetArea(ci::Area _dest)
{
	destRect = _dest;
}
