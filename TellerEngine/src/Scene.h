#pragma once
#include"Core.h"

namespace Teller {
	class Agent;

	class SceneModule :public ModuleCore {
	private:
		std::vector<std::shared_ptr<ModuleCore>>& agents_ =
			ModuleCore::children;
	public:
		SceneModule():ModuleCore(){};
		~SceneModule(){};
		void AddAgent(std::shared_ptr<Agent> _agent);
	};
}
