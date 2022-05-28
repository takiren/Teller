#pragma once

#include<memory>
#include<string>
namespace Teller {

	enum class ASSET_TYPE
	{
		NONE,
		Image,
		Audio
	};

	class Asset :public std::enable_shared_from_this<Asset> {
	protected:

		ASSET_TYPE type;
	public:
		Asset() = delete;
		Asset(std::string path):type(ASSET_TYPE::NONE){};
	};

	class Sprite :public Asset {
	protected:
	public:
		Sprite(std::string path) :Asset( path) {};
	};

	/*
	すべてのアセットはコンテンツマネージャーが管理し、
	エージェントやその他のUIは
	コンテンツマネージャーを介してデータを取得する。
	*/
}
