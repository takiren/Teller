#include"Animation.h"

void teller::AnimationSequencer::Update()
{
	for (auto& e : animators) {
		e->Update();
	}
}

void teller::AnimationSequencer::AddAnimator(std::unique_ptr<AnimatorCore> _animator)
{
	animators.push_back(std::move(_animator));
}

void teller::TextChanger::Initialize()
{
	auto cline = csvData->GetLine(currentline);
	callback_(cline.at(0), cline.at(1));
}

void teller::TextChanger::Update()
{

}

void teller::TextChanger::Next()
{
	currentline++;
}

void teller::TextChanger::SetText(std::string _speaker, std::string _text)
{
	callback_(_speaker, _text);
}

void teller::AnimatorCore::Update()
{
}

void teller::Circular::Initialize()
{
}

void teller::Circular::Update()
{
	theta += 0.02f;
	dpos_ = vec2(-sin(theta) * 100, cos(theta) * 100);
	callback_(dpos_, drot_, dscale_);
}

void teller::Repeat::Update()
{
	theta += 0.02f;
	dpos_ = vec2(-sin(theta) * 100, 0);
	callback_(dpos_);
}

void teller::CharacterAnimator::Update()
{
	callback_(vec2(0, 0), vec2(0, 0), vec2(0, 0), "");
}
