#pragma once
#include"Core.h"
#include"ModuleCore.h"
#include"Agent.h"

namespace Teller {
	class Agent;

	class SceneModule :public ModuleCore {
	private:
		std::vector<std::shared_ptr<Agent>> agents_;
	public:
		SceneModule():ModuleCore(){};
		~SceneModule(){};
		void AddAgent(std::shared_ptr<Agent> _agent);
	};
}
