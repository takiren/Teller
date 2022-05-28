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
	���ׂẴA�Z�b�g�̓R���e���c�}�l�[�W���[���Ǘ����A
	�G�[�W�F���g�₻�̑���UI��
	�R���e���c�}�l�[�W���[����ăf�[�^���擾����B
	*/
}
