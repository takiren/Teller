#include "TAgent.h"


void teller::TAgentBase::GetVariant(Variant _variant)
{
}

void teller::TAgentBase::SendVariant(Variant _variant)
{
}

void teller::TAgentBase::SendData(json _jdata)
{

}

void teller::TAgentBase::PushData(json data)
{
}


void teller::TCharacter::LoadCharacterImage(fs::path _path, std::string _characterName)
{
	json j;

	fs::path ipath = _path;
	std::ifstream i(ipath);
	if (i) i >> j;
	else {
		std::cout << "File not found : " << ipath.string() << std::endl;
		return;
	}

	auto nameInJson = j["name"].get<std::string>();
	if (nameInJson == _characterName) {
		for (auto& je : j["file"].items()) {
			auto imgpath = _path.remove_filename();
			imgpath /= fs::path(je.value().get<std::string>());

		}
	}
}

void teller::TCharacter::LoadSprite(fs::path _path)
{
	std::cout << "Trying to load : " << _path.string() << std::endl;
	charSpritesMap_[_path.filename().string()] = std::make_unique<Sprite>(_path);
	if (charSpritesMap_.size() == 1)
		currentSprite_ = _path.filename().string();
}

void teller::TCharacter::PushData(json jdata)
{
	if (jdata.contains("SpriteName")) {
		currentSprite_ = jdata["SpriteName"].get <std::string>();
	}

	if (jdata.contains("diff")) {
		auto& jdiff = jdata["diff"];
		if (jdiff.contains("pos")) {
			auto& jdiffpos = jdiff["pos"];
		}

	}
}

void teller::TCharacter::Update()
{
}

void teller::TCharacter::SetUp()
{
}

void teller::TCharacter::Draw()
{
	Rectf destRect = Rectf(charSpritesMap_[currentSprite_]->texture_->getBounds() + position_);
	gl::draw(charSpritesMap_[currentSprite_]->texture_, destRect);
}
