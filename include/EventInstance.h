#ifndef _TELLERENGINE_SRC_EVENTINSTANCE_H_
#define _TELLERENGINE_SRC_EVENTINSTANCE_H_


#include<map>
#include<string>
#include<vector>
#include<nlohmann/json.hpp>

#include"Tree.h"
#include"Variant.h"
#include"Agent.h"

namespace teller {

	class EventInstanceBase {
	private:
	protected:
		//‚±‚ê‚ªtrue‚É‚È‚Á‚½‚çEvent‚ðŠJŽn
		bool bFired = false;
		std::vector<Variant> variables;

	public:
		EventInstanceBase() = default;
		virtual ~EventInstanceBase() = default;

		virtual bool Tick() { 
			if (bFired)
				if (Update())
					return true; 
		}
		virtual bool Update() {}
	};

	template<class TARGET>
	class EventInstance :public EventInstanceBase {
	private:
	protected:
		std::shared_ptr<TARGET> targetRef;
	public:
		EventInstance() :
			EventInstanceBase() {};
	};

	class EpisodeEventInstance :public EventInstance<AgentCore> {
	private:
	protected:
	public:
	};
	
	template<class T>
	class EventSequencer {
	private:
	protected:
	public:
	};

}

#endif // !_TELLERENGINE_SRC_EVENTINSTANCE_H_