#pragma once
#include "Core.h"
#include<filesystem>
#include<memory>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include"cinder/ImageIo.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include"cinder/Log.h"
#include <list>
#include "cinder/CinderImGui.h"
#include"cinder/ImageIo.h"
#include"TellerCore.h"

namespace fs = std::filesystem;
using namespace Teller;
namespace Teller {
	class TellerCore;

	enum class ASSET_TYPE
	{
		Image,
		Audio
	};

	class Asset :public std::enable_shared_from_this<Asset> {
	protected:

		ASSET_TYPE type;
		virtual void Init();
	public:
		Asset() = default;
	};

	class Sprite :public Asset {
	protected:
		void Init() override;
	public:
		Sprite()=default;
	};

	/*
	すべてのアセットはコンテンツマネージャーが管理し、
	エージェントやその他のUIは
	コンテンツマネージャーを介してデータを取得する。
	*/
}
