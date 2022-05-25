#pragma once
#include "Game.h"
#include"Agent.h"

namespace Teller {
	class SceneModule :public ModuleCore{
	private:
		std::vector<std::shared_ptr<Agent>> agents_;
	public:
		void Tick();
		void AddAgent(std::shared_ptr<Agent>&& _agent);
	};
}

