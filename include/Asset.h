#pragma once
#include"cinder/Cinder.h"
#include"cinder/gl/gl.h"
#include"cinder/CinderImGui.h"

#include<memory>
#include<string>
#include<filesystem>
namespace teller {
	using namespace ci;
	using namespace ci::app;
	using namespace std;
	namespace fs = std::filesystem;

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
		fs::path filepath_;
	public:
		std::string name_;
		uint64_t ID_;
		gl::TextureRef texture_;
		Sprite() = delete;
		Sprite(fs::path _path) :
			Asset(ASSET_TYPE::Image),
			filepath_(_path),
			texture_(gl::Texture::create(loadImage(_path))),
			ID_((uint64_t)this)
		{};
	};

	/*
	すべてのアセットはコンテンツマネージャーが管理し、
	エージェントやその他のUIは
	コンテンツマネージャーを介してデータを取得する。
	*/
}
