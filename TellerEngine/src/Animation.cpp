#include"Animation.h"

void Teller::Animator::SetDeltaTime(float& _deltaTime)
{
	deltaTime_ = _deltaTime;
}

//–¢ŽÀ‘•
void Teller::Animator::Animate(vec2&, vec2&, vec2&)
{
}

void Teller::Animator::Attach(int key, AnimatorTargetFunc& _destination)
{
	callbackMap_[key] = _destination;
}

void Teller::Animator::AnimateD(std::function<void(vec2&, vec2&, vec2&)>)
{
	
}

void Teller::Circular::AnimateD(std::function<void(vec2&, vec2&, vec2&)>)
{

}
