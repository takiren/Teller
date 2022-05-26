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

	template<class TYPE>
	class ContentManager {
	private:
		std::vector<std::shared_ptr<TYPE>> contents;
	public:
		ContentManager() = default;
		~ContentManager() = default;
		//�R�s�[�֎~
		ContentManager(const ModuleCore&) = delete;
		ModuleCore& operator=(const ModuleCore&) = delete;
		//���[�u�͂����B
		ModuleCore& operator=(ModuleCore&&) = default;

	};

}
