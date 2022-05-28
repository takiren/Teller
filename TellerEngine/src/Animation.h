#pragma once
#include<functional>
#include<map>
#include"cinder/Cinder.h"
#include"cinder/gl/gl.h"
#include"Agent.h"

namespace Teller {
	using namespace ci;
	class Animator {
	protected:
		//A letter "d" means diff
		vec2 dpos_;
		vec2 drot_;
		vec2 dscale_;

		std::weak_ptr<Agent> target;
		float deltaTime_;
		using AnimatorTargetFunc = std::function<void(vec2&, vec2&, vec2&)>;
		AnimatorTargetFunc callback_;
		virtual void AnimateInternal();
		virtual void Animate(vec2&, vec2&, vec2&);
	public:
		Animator() :
			deltaTime_(0.0),
			dpos_(vec2(0)),
			drot_(vec2(0)),
			dscale_(1)
		{};

		Animator(const Animator&) = delete;
		Animator& operator=(const Animator&) = delete;

		Animator& operator=(Animator&&) = default;

		void SetDeltaTime(float _deltaTime);

		virtual void Update();

		virtual void AttachToAgent(std::weak_ptr<Agent> _agent);
	};

	class Circular :public Animator {
	private:
		float theta;
	public:
		Circular() :Animator(), theta(0) {};
		void AnimateInternal() override;
	};

	class AnimationSequencer {
		std::vector<std::unique_ptr<Animator>> animators;
	public:
		AnimationSequencer() = default;
		~AnimationSequencer() = default;

		AnimationSequencer(const AnimationSequencer&) = delete;
		AnimationSequencer& operator=(const AnimationSequencer&) = delete;

		AnimationSequencer& operator=(AnimationSequencer&&) = default;

		void Update();
		void AddAnimator(std::unique_ptr<Animator> _animator);
	};
}