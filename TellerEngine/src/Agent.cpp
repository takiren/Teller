#include "Agent.h"

void teller::RectAgent::Tick()
{
	Rectf rf = Rectf(Area(vec2(0, 0), vec2(200, 200) + position_));
	gl::drawSolidRect(rf);
}

void teller::RectAgent::CallBackListener(vec2 _pos, vec2 _rot, vec2 _scale)
{
	position_ += _pos * 0.01f;
	rotation_ += _rot * 0.01f;
	scale_ += _scale * 0.01f;
}

void teller::MainTextArea::Tick()
{
	gl::drawString(speaker_, vec2(100, 470), ci::Color(1, 1, 1), ci::Font("", 40));
	gl::drawString(text_, vec2(100, 520), ci::Color(1, 1, 1), ci::Font("", 30));
}

void teller::MainTextArea::CallBackListener(std::string _speaker, std::string _text)
{
	speaker_ = _speaker;
	text_ = _text;
}

void teller::AgentCore::Tick()
{
}

void teller::AgentCore::Draw()
{
}

void teller::AgentCore::SetDeltaTime(float _deltaTime)
{

}

void teller::Character::SetCharacterSprite(std::string _key, std::shared_ptr<Sprite> _sprite)
{
}

void teller::Character::Tick()
{

}

void teller::Character::AddCharacterAppearance(std::string  _key, std::unique_ptr<CharacterAppearance> _appearance)
{
	appearanceMap_[_key] = std::move(_appearance);
}


void teller::Character::CallBackListener(vec2 _pos, vec2 _rot, vec2 _scale, CharacterAppearance _apperance)
{
	position_ += _pos * deltaTime_;
	rotation_ += _rot * deltaTime_;
	scale_ += _scale * deltaTime_;
}

void teller::Kappa::Tick()
{
	Rectf destRect = Rectf(sprite_->texture_->getBounds() + position_);
	gl::draw(sprite_->texture_, destRect);
}

void teller::Kappa::CallBackListener(vec2 _pos)
{
	position_ += _pos * deltaTime_;
}

void teller::CharacterAppearance::SetItem(CHARACTER_ITEM _enumitem, fs::path _path)
{
}

void teller::CharacterAppearance::SetItem(CHARACTER_ITEM _enumitem, CharacterItem _item)
{

}

void teller::CharacterAppearance::SetPosition(vec2 _position)
{
}

void teller::CharacterItem::Draw()
{
}

void teller::CharacterSimple::Initialize(fs::path _path)
{
	json j;
	std::ifstream jsonEpisode(_path.string(), std::ios::in);
	jsonEpisode >> j;
	name_ = j["name"];
	for (auto& e : j["file"].items()) {

	}
}

void teller::CharacterSimple::Tick()
{
	Rectf destRect = Rectf(charSprites_[currentSprite]->texture_ ->getBounds() + position_);
	gl::draw(charSprites_[currentSprite]->texture_, destRect);
}


void teller::CharacterSimple::AddCharacterSprite(std::string _key, std::unique_ptr<Sprite> _sprite)
{
	charSprites_[_key] = std::move(_sprite);
}

std::vector<std::string> teller::CharacterSimple::GetKeys()
{
	std::vector<std::string> keys;

	for (auto& e : charSprites_) {
		keys.push_back(e.first);
	}
	return keys;
}

void teller::CharacterSimple::CallBackListener(vec2 _pos, vec2 _rot, vec2 _scale, std::string _key)
{
	SetCurrentSprite(_key);
}
