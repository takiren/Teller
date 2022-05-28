#include "Agent.h"

void Teller::Character::Tick(float& deltaTime)
{
	//Rectf destRect = Rectf(sprite_.lock()->texture->getBounds()).getCenteredFit(getWindowBounds(), true).scaledCentered(0.85f);
	//gl::draw(sprite_.lock()->texture, destRect);
}

void Teller::Character::SetSprite()
{
}

void Teller::Character::Update()
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

void Teller::Agent::Animate(vec2& _dpos, vec2& _drot, vec2& _dscale)
{
	position_ += deltaTime_;
	rotation_ += deltaTime_;
	scale_ += deltaTime_;
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

void Teller::Text::Initialize()
{
}

void Teller::Text::Tick(float& deltaTime)
{
	/*count++;
	Agent::Tick();
	Agent::Update();
	if (count % 200 == 0) {
		currentLine++;
	}
	if (currentLine >= episodes.back().data.size()) {
		currentLine = episodes.back().data.size() - 1;
	}
	auto size = cinder::app::getWindowSize();
	currentText =
		episodes.back().data.at(currentLine).at(1);
	gl::drawString(currentText, vec2(size[0] * 0.3, size[1] * 0.7), Color(1, 1, 1), Font("", 50));*/
}

void Teller::Text::Update()
{
}

void Teller::RectAgent::Tick(float& deltaTime)
{
	Rectf rf = Rectf(Area(vec2(0, 0), vec2(300, 300)));
	gl::drawSolidRect(rf, vec2(40, 40), vec2(400, 400));
}
