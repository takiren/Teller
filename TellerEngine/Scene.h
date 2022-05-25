#pragma once
#include "Game.h"
#include"Agent.h"

namespace Teller {
	class SceneModule :public ModuleCore {
	private:
		std::vector<std::shared_ptr<ModuleCore>>& agents_ =
			ModuleCore::children;
	public:
		SceneModule():ModuleCore(){};
		~SceneModule(){};
		void Tick() override;
		void AddAgent(std::shared_ptr<Agent>&& _agent);
	};
}

