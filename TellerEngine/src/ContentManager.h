#pragma once
#include<string>
#include<map>
#include<memory>
#include<vector>

#include"Episode.h"
#include"Asset.h"

namespace Teller {

	template<class TYPE>
	class ContentsManager {
	private:
		//�f�[�^��map�ŊǗ�
		//key=std::string�̓t�@�C�����ATYPE�͌^

	public:
		std::map<std::string, std::shared_ptr<TYPE>> contents_;
		ContentsManager();
		//�R�s�[�֎~
		ContentsManager(const ContentsManager&) = delete;
		ContentsManager& operator=(const ContentsManager&) = delete;
		//���[�u�͋��B
		ContentsManager& operator=(ContentsManager&&) = default;

		void AddContent(const std::string key);
		std::shared_ptr<TYPE> GetContent(const std::string key);

		//std::map�̃R���e���c�̃L�[��Ԃ��B
		std::vector<std::string> GetKeys();
		std::map<std::string, std::shared_ptr<TYPE>> contents()const { return contents_; };

	};

	template<class TYPE>
	inline ContentsManager<TYPE>::ContentsManager()
	{
	}

	template<class TYPE>
	inline void ContentsManager<TYPE>::AddContent(std::string key)
	{
		contents_[key] = std::move( std::make_unique<TYPE>(key));
	}

	template<class TYPE>
	inline std::shared_ptr<TYPE> ContentsManager<TYPE>::GetContent(std::string key)
	{
		return contents_[key];
	}

	template<class TYPE>
	inline std::vector<std::string> ContentsManager<TYPE>::GetKeys()
	{
		auto keys = std::vector<std::string>();
		for (auto iter = contents_.begin(); iter != contents_.end(); ++iter) {
			keys.push_back(iter->first);
		}
		return keys;
	}

	template class ContentsManager<CSVLoader>;
	template class ContentsManager<Sprite>;
	template class ContentsManager<Episode>;
}
