#include "Agent.h"

void teller::RectAgent::Update()
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

void teller::MainTextArea::Update()
{
	gl::drawString(speaker_, vec2(100, 470), ci::Color(1, 1, 1), ci::Font("", 40));
	gl::drawString(text_, vec2(100, 520), ci::Color(1, 1, 1), ci::Font("", 30));
}

void teller::MainTextArea::CallBackListener(std::string _speaker, std::string _text)
{
	speaker_ = _speaker;
	text_ = _text;
}

void teller::AgentCore::UpdateInternal()
{
}

void teller::AgentCore::Update()
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

void teller::Character::Update()
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

void teller::Kappa::Update()
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

void teller::CharacterSimple::Initialize(fs::path _path, std::string _charactername)
{
	//jsonを読み込んでスプライトを生成
	json j;
	//std::vector<fs::path> entries = cppglob::glob(_path / fs::path("*/*.json"));

	fs::path ipath = _path;
	//json検索
	std::ifstream i(ipath);
	if (i) i >> j;
	else {
		std::cout << "File not found : " << ipath.string() << std::endl;
		return;
	}
	auto nameInJson = j["name"].get<std::string>();

	if (nameInJson == _charactername) {
		for (auto je : j["file"].items()) {
			auto imgpath = _path.remove_filename();

			imgpath /= fs::path(je.value().get<std::string>());
			LoadSprite(imgpath);
		}
	}
	else {
		std::cout << "Failed" << std::endl;
	}

}

void teller::CharacterSimple::LoadSprite(fs::path _path)
{
	std::cout << "Trying to load : " << _path.string() << std::endl;

	charSprites_[_path.filename().string()] = std::make_unique<Sprite>(_path);
	if (charSprites_.size() == 1) currentSprite = _path.filename().string();
}

void teller::CharacterSimple::Update()
{
	Rectf destRect = Rectf(charSprites_[currentSprite]->texture_->getBounds() + position_);
	gl::draw(charSprites_[currentSprite]->texture_, destRect);
	std::cout << "draw" << std::endl;
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

void teller::BackGroundImage::Update()
{
}
