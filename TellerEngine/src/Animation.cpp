#include"Animation.h"

void Teller::Animator::SetDeltaTime(float& _deltaTime)
{
	deltaTime_ = _deltaTime;
}

//–¢ŽÀ‘•
void Teller::Animator::Animate(vec2&, vec2&, vec2&)
{
}

void Teller::Animator::Attach(int key, AnimatorTargetFunc _destination)
{
	callbackMap_[key] = _destination;
}

void Teller::Animator::Update()
{
	for (auto iter = callbackMap_.begin(); 
		iter != callbackMap_.end(); 
		++iter) {
		iter->second(dpos_, drot_, dscale_);
	}
}

void Teller::Animator::AnimateInternal()
{
}

void Teller::Animator::AnimateD(std::function<void(vec2&, vec2&, vec2&)>& callback)
{
	//Do nothing
}

void Teller::Circular::AnimateD(std::function<void(vec2&, vec2&, vec2&)>& callback)
{
	theta += deltaTime_ * 0.001;
	dpos_ = vec2(sin(theta) * deltaTime_, cos(theta) * deltaTime_);
	callback(dpos_, drot_, dscale_);
}
