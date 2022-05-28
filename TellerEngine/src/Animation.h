#pragma once
#include<functional>
#include<map>
#include"cinder/Cinder.h"
#include"cinder/gl/gl.h"
#include"TellerCore.h"

namespace Teller {
	using namespace ci;

	class Animator {
	protected:
		//A letter "d" means diff
		vec2 dpos_;
		vec2 drot_;
		vec2 dscale_;

		float deltaTime_;
		using AnimatorTargetFunc = std::function<void(vec2&, vec2&, vec2&)>;
		std::map<int, AnimatorTargetFunc> callbackMap_;
	public:
		Animator() :
			deltaTime_(0.0),
			dpos_(vec2(0)),
			drot_(vec2(0)),
			dscale_(1)
		{};
		void SetDeltaTime(float& _deltaTime);
		virtual void Animate(vec2&, vec2&, vec2&);

		virtual void Attach(int key, AnimatorTargetFunc _destination);
		virtual void Update();
		virtual void AnimateInternal();

		virtual void AnimateD(std::function<void(vec2&, vec2&, vec2&)>& callback);
	};

	class Circular :public Animator {
	private:
		float theta;
	public:
		Circular() :Animator(), theta(0){};
		void AnimateD(std::function<void(vec2&, vec2&, vec2&)>& callback) override;
	};
}