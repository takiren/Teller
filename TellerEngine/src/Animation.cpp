#include"Animation.h"

//–¢ŽÀ‘•

//void Teller::Animator::Update()
//{
//	AnimateInternal();
//	
//	callback_(dpos_, drot_, dscale_);
//	ImGui::Text("Diff Pos%f %f", dpos_[0], dpos_[1]);
//}

//void Teller::Animator::AttachToAgent(std::shared_ptr<Agent> _agent)
//{
//	std::weak_ptr<Agent> wp = _agent;
//	target = _agent;
//	callback_ = [&](vec2 _dpos, vec2 _drot, vec2 _dscale) {target.lock()->Animate(_dpos, _drot, _dscale); };
//}
//
//void Teller::Circular::AnimateInternal()
//{
//	theta += deltaTime_ * 0.01;
//	dpos_ = vec2(-sin(theta)*1, cos(theta)*1);
//}

void Teller::AnimationSequencer::Update()
{
	for (auto& e : animators) {
		e->Update();
	}
}

void Teller::AnimationSequencer::AddAnimator(std::unique_ptr<AnimatorCore> _animator)
{
	animators.push_back(std::move(_animator));
}

void Teller::TextChanger::Update()
{
	
}

void Teller::TextChanger::Next()
{
	currentline++;
}

void Teller::AnimatorCore::Update()
{
}

void Teller::Circular::Update()
{
	theta += 0.01f;
	dpos_ = vec2(-sin(theta) * 100, cos(theta) * 100);
	callback_(dpos_, drot_, dscale_);
}
