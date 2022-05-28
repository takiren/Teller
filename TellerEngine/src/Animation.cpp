#include"Animation.h"

void Teller::Animator::SetDeltaTime(float _deltaTime)
{
	deltaTime_ = _deltaTime;
}

//–¢ŽÀ‘•
void Teller::Animator::Animate(vec2&, vec2&, vec2&)
{
}


void Teller::Animator::Update()
{
	AnimateInternal();
	callback_(dpos_, drot_, dscale_);
}

void Teller::Animator::AnimateInternal()
{
}

void Teller::Animator::AttachToAgent(std::weak_ptr<Agent> _agent)
{
	callback_ = [&](vec2,vec2,vec2) {_agent.lock()->Animate(dpos_, drot_, dscale_); };
}

void Teller::Circular::AnimateInternal()
{
	theta += deltaTime_ * 0.001;
	dpos_ = vec2(sin(theta) * deltaTime_, cos(theta) * deltaTime_);
}

void Teller::AnimationSequencer::Update()
{
	for (auto& e : animators) {
		e->Update();
	}
}

void Teller::AnimationSequencer::AddAnimator(std::unique_ptr<Animator> _animator)
{
	animators.push_back(std::move(_animator));
}
