#pragma once

#include<memory>

namespace Teller {

	enum class ASSET_TYPE
	{
		Image,
		Audio
	};

	class Asset :public std::enable_shared_from_this<Asset> {
	protected:

		ASSET_TYPE type;
	public:
		Asset() = default;
	};

	class Sprite :public Asset {
	protected:
	public:
		Sprite() :Asset() {};
	};

	/*
	すべてのアセットはコンテンツマネージャーが管理し、
	エージェントやその他のUIは
	コンテンツマネージャーを介してデータを取得する。
	*/
}
