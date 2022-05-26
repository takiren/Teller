#pragma once
#include<string>
#include<memory>
#include<map>

namespace Teller {
	template<class TYPE>
	class ContentManager {
	private:
		std::map<std::string, std::shared_ptr<TYPE>> contents;
	public:

		ContentManager() = default;
		~ContentManager() = default;
		//コピー禁止
		ContentManager(const ContentManager&) = delete;
		ContentManager& operator=(const ContentManager&) = delete;
		//ムーブは許可。
		ContentManager& operator=(ContentManager&&) = default;

		void AddContent(std::string key, TYPE content);
		std::shared_ptr<TYPE> GetContent(std::string key);
		std::vector<std::string> GetKeys();
		std::map<std::string, std::shared_ptr<TYPE>> contents()const { return contents; };
	};

	template<class TYPE>
	inline void ContentManager<TYPE>::AddContent(std::string key, TYPE content)
	{
		content[key] = content;
	}

	template<class TYPE>
	inline std::shared_ptr<TYPE> ContentManager<TYPE>::GetContent(std::string key)
	{
		return contents.at(key)->shared_from_this();
	}

	template<class TYPE>
	inline std::vector<std::string> ContentManager<TYPE>::GetKeys()
	{
		auto keys = std::vector<std::string>();
		for (auto iter = contents.begin(); iter != end; iter++) {
			keys.push_back(iter->first);
		}
		return keys;
	}
}
