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
