#pragma once
#include"Core.h"
#include"Agent.h"

namespace Teller {
	class Animation;
	class Animator{
	private:
		std::weak_ptr<Agent> parent;
	};
	class Animation {
		std::weak_ptr<Agent> parent;
		
	public:
		Animation(std::shared_ptr<Agent>&& agent);
	};
}