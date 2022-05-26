#pragma once
#include"Core.h"
#include"ModuleCore.h"

namespace Teller {
	class Agent;

	class SceneModule :public ModuleCore {
	private:
	public:
		SceneModule():ModuleCore(){};
		~SceneModule(){};
		void AddAgent(std::shared_ptr<Agent> _agent);
	};
}
