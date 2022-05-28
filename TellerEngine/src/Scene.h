#pragma once
#include<vector>
#include<memory>

#include"ModuleCore.h"
#include"Agent.h"

namespace Teller {
	class SceneModule :	public ModuleCore
	{
	private:
		std::vector<std::shared_ptr<Animator>> animators_;
		std::vector<std::shared_ptr<Agent>> agents_;
	public:
		SceneModule():ModuleCore(){};
		~SceneModule(){};
		void Tick(float& deltaTime) override;
		void AddAgent(std::shared_ptr<Agent>&& _agent);
	};
}
