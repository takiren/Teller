#pragma once
#include<string>
#include<map>
#include<memory>
#include<vector>

#include"Episode.h"
#include"Asset.h"

namespace Teller {
	template< class TYPE>
	class ContentsManager {
	private:
		//�f�[�^��map�ŊǗ�
		//key=uint64_t��ID�ATYPE�͌^

	public:
		std::map < uint64_t, std::shared_ptr<TYPE>> contents_;
		ContentsManager() = default;
		//�R�s�[�֎~
		ContentsManager(const ContentsManager&) = delete;
		ContentsManager& operator=(const ContentsManager&) = delete;
		//���[�u�͋��B
		ContentsManager& operator=(ContentsManager&&) = default;

		void LoadContent(const std::string _path);
		std::shared_ptr<TYPE> GetContent(uint64_t key);

		void AddContent(uint64_t _key, std::unique_ptr<TYPE> _content);

		std::string GetName(uint64_t _key);

		std::vector<std::string> GetNames();

		std::map<uint64_t, std::string> GetKeyWithValue();

		//std::map�̃R���e���c�̃L�[��Ԃ��B
		std::vector<uint64_t> GetKeys();
		std::map<uint64_t, std::shared_ptr<TYPE>> contents()const { return contents_; };
	};

	template<class TYPE>
	inline void ContentsManager<TYPE>::LoadContent(const std::string _path)
	{
		auto n = std::make_unique<TYPE>(_path);
		contents_[n->ID_] = std::move(n);
	}

	template< class TYPE>
	inline std::shared_ptr<TYPE> ContentsManager<TYPE>::GetContent(uint64_t key)
	{
		return contents_[key];
	}

	template<class TYPE>
	inline void ContentsManager<  TYPE>::AddContent(uint64_t _key, std::unique_ptr<TYPE> _content)
	{
		contents_[_key] = std::move(_content);
	}

	template<class TYPE>
	inline std::string ContentsManager<TYPE>::GetName(uint64_t _key)
	{
		return contents_[_key]->name_;
	}

	template<class TYPE>
	inline std::vector<std::string> ContentsManager<TYPE>::GetNames()
	{
		std::vector < std::string> namevec_;
		for (auto& e : contents_) {
			namevec_.push_back(e.second->name_);
		}
		return std::vector<std::string>();
	}

	template<class TYPE>
	inline std::map<uint64_t, std::string> ContentsManager<TYPE>::GetKeyWithValue()
	{
		std::map<uint64_t, std::string> mp;
		for (auto& e : contents_) {
			mp[e.first] = e.second->name_;
		}
		return mp;
	}

	template<class TYPE>
	inline std::vector<uint64_t> ContentsManager<TYPE>::GetKeys()
	{
		auto keys = std::vector<uint64_t>();
		if (contents_.size() == 0) return keys;
		for (auto iter = contents_.begin(); iter != contents_.end(); ++iter) {
			keys.push_back(iter->first);
		}
		return keys;
	}

	template class ContentsManager<CSVLoader>;
	template class ContentsManager<Sprite>;
	template class ContentsManager<Episode>;
}
