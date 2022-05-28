#include "Agent.h"


void Teller::Character::Tick()
{
}

void Teller::Character::SetSprite()
{
}


void Teller::Character::GetDraw()
{
}
////Animatorインスタンスのポインタとコールバックを登録。
//void Teller::Agent::AttachAnimator(std::shared_ptr<Animator>&& _animator, int key)
//{
//	animatorMap_[key] = _animator;
//	animatorCallBackMap_[key] = [&]() { return animatorMap_[key]->Animate(size_, position_, rotation_); };
//}
//
//void Teller::Agent::AnimateInternal(int key, float factor)
//{
//
//}

void Teller::Agent::Animate(vec2 _dpos, vec2 _drot, vec2 _dscale)
{
	position_ += _dpos;
	rotation_ += _drot;
	scale_ += _dscale;
}


void Teller::Agent::Scale()
{
}

void Teller::Agent::Rotate()
{
}

void Teller::Agent::MessageHandler()
{
}

void Teller::Agent::SetAnimation()
{
}

void Teller::Agent::Tick()
{
}

void Teller::Text::Initialize()
{
}

void Teller::RectAgent::Tick()
{
	Rectf rf = Rectf(Area(vec2(0, 0), vec2(300, 300)));
	gl::drawSolidRect(rf, vec2(40, 40), vec2(400, 400));
}
