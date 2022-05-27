#pragma once
#include"Core.h"
#include"Episode.h"
#include"Asset.h"

namespace Teller {

	template<class TYPE>
	class ContentsManager {
	private:
		//std::map<int, std::function<void()>&> callbackMap;

		std::map<std::string, std::shared_ptr<TYPE>> contents_;
	public:
		ContentsManager();
		//コピー禁止
		ContentsManager(const ContentsManager&) = delete;
		ContentsManager& operator=(const ContentsManager&) = delete;
		//ムーブは許可。
		ContentsManager& operator=(ContentsManager&&) = default;

		void AddContent(const std::string key, const std::shared_ptr<TYPE> content);
		std::shared_ptr<TYPE> GetContent(const std::string key);

		//std::mapのコンテンツのキーを返す。
		std::vector<std::string> GetKeys();
		std::map<std::string, std::shared_ptr<TYPE>> contents()const { return contents_; };

	};

	template<class TYPE>
	inline ContentsManager<TYPE>::ContentsManager()
	{
	}

	template<class TYPE>
	inline void ContentsManager<TYPE>::AddContent(std::string key, std::shared_ptr<TYPE> content)
	{
		contents_[key] = content;
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
		for (auto iter = contents_.begin(); iter != contents_.end(); iter++) {
			keys.push_back(iter->first);
		}
		return keys;
	}

	template class ContentsManager<CSVLoader>;
	template class ContentsManager<Sprite>;
	template class ContentsManager<Episode>;
}
