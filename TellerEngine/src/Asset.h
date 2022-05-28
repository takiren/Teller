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
	���ׂẴA�Z�b�g�̓R���e���c�}�l�[�W���[���Ǘ����A
	�G�[�W�F���g�₻�̑���UI��
	�R���e���c�}�l�[�W���[����ăf�[�^���擾����B
	*/
}
