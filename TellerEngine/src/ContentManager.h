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
		//データはmapで管理
		//key=std::stringはファイル名、TYPEは型

	public:
		std::map<std::string, std::shared_ptr<TYPE>> contents_;
		ContentsManager() = default;
		//コピー禁止
		ContentsManager(const ContentsManager&) = delete;
		ContentsManager& operator=(const ContentsManager&) = delete;
		//ムーブは許可。
		ContentsManager& operator=(ContentsManager&&) = default;

		void LoadContent(const std::string key);
		std::shared_ptr<TYPE> GetContent(const std::string key);

		void AddContent(std::string _key, std::unique_ptr<TYPE> _content);

		//std::mapのコンテンツのキーを返す。
		std::vector<std::string> GetKeys();
		const std::map<std::string, std::shared_ptr<TYPE>>& contents()const { return contents_; };
	};


	template<class TYPE>
	inline void ContentsManager<TYPE>::LoadContent(std::string key)
	{
		contents_[key] = std::move(std::make_unique<TYPE>(key));
	}

	template<class TYPE>
	inline std::shared_ptr<TYPE> ContentsManager<TYPE>::GetContent(std::string key)
	{
		return contents_[key];
	}

	template<class TYPE>
	inline void ContentsManager<TYPE>::AddContent(std::string _key, std::unique_ptr<TYPE> _content)
	{
		contents_[_key] = std::move(_content);
	}

	template<class TYPE>
	inline std::vector<std::string> ContentsManager<TYPE>::GetKeys()
	{
		auto keys = std::vector<std::string>(0, "");
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
