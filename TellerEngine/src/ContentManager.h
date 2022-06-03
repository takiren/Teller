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
		//データはmapで管理
		//key=uint64_tはID、TYPEは型

	public:
		std::map < uint64_t, std::shared_ptr<TYPE>> contents_;
		ContentsManager() = default;
		//コピー禁止
		ContentsManager(const ContentsManager&) = delete;
		ContentsManager& operator=(const ContentsManager&) = delete;
		//ムーブは許可。
		ContentsManager& operator=(ContentsManager&&) = default;

		void LoadContent(const std::string _path);
		std::shared_ptr<TYPE> GetContent(uint64_t key);

		void AddContent(uint64_t _key, std::unique_ptr<TYPE> _content);
		

		//std::mapのコンテンツのキーを返す。
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
