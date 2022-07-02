#ifndef _TELLERENGINE_SRC_EVENTINSTANCE_H_
#define _TELLERENGINE_SRC_EVENTINSTANCE_H_
#include<map>
#include<string>
#include<vector>
#include"Tree.h"
#include"Variant.h"

namespace teller {
	class EventDataPack {
	};

	class EventInstanceBase {
	private:
	protected:
		//‚±‚ê‚ªtrue‚É‚È‚Á‚½‚çEvent‚ðŠJŽn
		bool bFired = false;
	public:
		EventInstanceBase() = default;
		virtual ~EventInstanceBase() = default;

		virtual void Tick() { if (bFired) Update(); }
		virtual void Update(){}
	};


	template<class TARGET>
	class EventInstance :public EventInstanceBase {
	private:
		std::shared_ptr<TARGET> target;

	protected:
	public:
		EventInstance() :
			EventInstanceBase() {};

		void Update() override;
	};

	template<class T>
	inline void EventInstance<T>::Update()
	{
		target->Update();
	}

}

#endif // !_TELLERENGINE_SRC_EVENTINSTANCE_H_