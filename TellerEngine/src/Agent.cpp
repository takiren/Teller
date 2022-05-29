#include "Agent.h"

void Teller::RectAgent::Tick()
{
	Rectf rf = Rectf(Area(vec2(0, 0), vec2(200, 200) + position_));
	gl::drawSolidRect(rf);
}

void Teller::RectAgent::CallBackListener(vec2 _pos, vec2 _rot, vec2 _scale)
{
	position_ += _pos * 0.01f;
	rotation_ += _rot * 0.01f;
	scale_ += _scale * 0.01f;
}

void Teller::MainTextArea::Tick()
{
	gl::drawString(speaker_, vec2(100, 470), ci::Color(1, 1, 1), ci::Font("", 40));
	gl::drawString(text_, vec2(100, 520), ci::Color(1, 1, 1), ci::Font("", 30));
}

void Teller::MainTextArea::CallBackListener(std::string _speaker, std::string _text)
{
	speaker_ = _speaker;
	text_ = _text;
}

void Teller::AgentCore::Tick()
{
}

void Teller::AgentCore::SetDeltaTime(float _deltaTime)
{

}

void Teller::Character::Tick()
{
}

void Teller::Character::CallBackListener(vec2 _pos, vec2 _rot, vec2 _scale)
{
	position_ += _rot * deltaTime_;
	position_ += _scale * deltaTime_;
	position_ += _pos * deltaTime_;
}
