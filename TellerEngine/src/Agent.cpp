#include "Agent.h"

void Teller::Character::Tick()
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

void Teller::Agent::Move()
{
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

void Teller::Text::Tick()
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
