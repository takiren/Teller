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
	���ׂẴA�Z�b�g�̓R���e���c�}�l�[�W���[���Ǘ����A
	�G�[�W�F���g�₻�̑���UI��
	�R���e���c�}�l�[�W���[����ăf�[�^���擾����B
	*/
}
