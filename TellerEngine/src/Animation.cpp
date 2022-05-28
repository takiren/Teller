#include"Animation.h"

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

void Teller::TextChanger::Initialize()
{
	auto cline = csvData->GetLine(currentline);
	callback_(cline.at(0), cline.at(1));
}

void Teller::TextChanger::Update()
{
	count_++;
	if (count_ % 300 == 0) {
		Next();
		auto cline = csvData->GetLine(currentline);
		callback_(cline.at(0), cline.at(1));
	}
	if (timer_.getSeconds() == 3) {
		Next();
		auto& cline = csvData.get()->GetCSVData().at(currentline);
		callback_(cline.at(0), cline.at(1));
	}
}

void Teller::TextChanger::Next()
{
	currentline++;
}

void Teller::AnimatorCore::Update()
{
}

void Teller::Circular::Initialize()
{
}

void Teller::Circular::Update()
{
	theta += 0.01f;
	dpos_ = vec2(-sin(theta) * 100, cos(theta) * 100);
	callback_(dpos_, drot_, dscale_);
}
