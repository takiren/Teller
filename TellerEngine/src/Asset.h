#pragma once
#include "Core.h"

namespace fs = std::filesystem;
using namespace Teller;
namespace Teller {
	enum class ASSET_TYPE
	{
		Image,
		Audio
	};

	class Asset :public std::enable_shared_from_this<Asset> {
	protected:
		fs::path filePath;
		ASSET_TYPE type;
		virtual void Init();
	public:
		Asset() = default;
		Asset(fs::path _path) :
			filePath(_path) {
		};
	};

	class Sprite :public Asset {
	protected:
		void Init() override;
	public:
		gl::TextureRef texture;
		Sprite(fs::path _path) :Asset(_path){};
	};

	

	/*
	すべてのアセットはコンテンツマネージャーが管理し、
	エージェントやその他のUIは
	コンテンツマネージャーを介してデータを取得する。
	*/
}
