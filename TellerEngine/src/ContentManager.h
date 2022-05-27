#pragma once
#include"Core.h"
#include"Episode.h"
#include"Asset.h"


namespace Teller {
	template<class TYPE>
	//using ContentT = std::shared_ptr<TYPE>;

	class ContentManager {
	private:
		std::map<std::string, std::shared_ptr<TYPE>> contents_;
	public:
		ContentManager();
		//コピー禁止
		ContentManager(const ContentManager&) = delete;
		ContentManager& operator=(const ContentManager&) = delete;
		//ムーブは許可。
		ContentManager& operator=(ContentManager&&) = default;

		void AddContent(const std::string key, const std::shared_ptr<TYPE> content);
		std::shared_ptr<TYPE> GetContent(const std::string key);
		std::vector<std::string> GetKeys();
		std::map<std::string, std::shared_ptr<TYPE>> contents()const { return contents_; };
	};

	template<class TYPE>
	inline ContentManager<TYPE>::ContentManager()
	{
	}

	template<class TYPE>
	inline void ContentManager<TYPE>::AddContent(std::string key, std::shared_ptr<TYPE> content)
	{
		contents_[key] = content;
	}

	template<class TYPE>
	inline std::shared_ptr<TYPE> ContentManager<TYPE>::GetContent(std::string key)
	{
		return contents_[key];
	}

	template<class TYPE>
	inline std::vector<std::string> ContentManager<TYPE>::GetKeys()
	{
		auto keys = std::vector<std::string>();
		for (auto iter = contents_.begin(); iter != contents_.end(); iter++) {
			keys.push_back(iter->first);
		}
		return keys;
	}

	template class ContentManager<CSVLoader>;
	template class ContentManager<Sprite>;
	template class ContentManager<Episode>;
}
