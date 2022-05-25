#include "Agent.h"

void Teller::Character::Tick()
{
	Rectf destRect = Rectf(csprite->texture->getBounds()).getCenteredFit(getWindowBounds(), true).scaledCentered(0.85f);
	gl::draw(csprite->texture, destRect);
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
	auto csv = std::make_unique<CSVLoader>(path_);
	episodes.emplace_back(csv->GetCSVData());
}

void Teller::Text::Tick()
{
	Agent::Tick();
	Agent::Update();
	if (count % 1000 == 0) {
		currentLine++;

	}
	if (currentLine > 12)currentLine = 12;

	currentText =
		episodes.back().data.at(currentLine).at(1);
	gl::drawString(currentText, vec2(500, 500));

}

void Teller::Text::Update()
{
}
