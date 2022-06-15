#pragma once
#include"cinder/Cinder.h"
#include"cinder/gl/gl.h"
#include"cinder/CinderImGui.h"

#include<memory>
#include<string>
namespace teller {
	using namespace ci;
	using namespace ci::app;
	using namespace std;
	enum class ASSET_TYPE
	{
		NONE,
		Image,
		Audio
	};

	class Asset :public std::enable_shared_from_this<Asset> {
	protected:
		ASSET_TYPE type_;
	public:
		Asset() = delete;
		Asset(ASSET_TYPE _type) :type_(_type) {};
	};

	class Sprite :public Asset {
	protected:
		std::string filepath_;
	public:
		std::string name_;
		uint64_t ID_;
		gl::TextureRef texture_;
		Sprite() = delete;
		Sprite(std::string path) :
			Asset(ASSET_TYPE::Image),
			filepath_(path),
			texture_(gl::Texture::create(loadImage(path))),
			ID_((uint64_t)this)
		{};
	};

	/*
	���ׂẴA�Z�b�g�̓R���e���c�}�l�[�W���[���Ǘ����A
	�G�[�W�F���g�₻�̑���UI��
	�R���e���c�}�l�[�W���[����ăf�[�^���擾����B
	*/
}
