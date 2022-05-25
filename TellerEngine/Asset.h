#pragma once
#include "Core.h"
namespace fs = std::filesystem;
namespace Teller {
	enum class ASSET_TYPE
	{
		Image,
		Audio
	};

	class Asset
	{
	protected:
		fs::path filePath;
		ASSET_TYPE type;
		void Init();
	public:
		Asset(fs::path _path) :
			filePath(_path) {
			Init();
		};
	};

	class Sprite :public Asset {
		void Init();
	public:
		gl::TextureRef texture;
		Sprite(fs::path _path) :Asset(_path) { Init(); };
	};

	class AssetLoader {

	};

}
