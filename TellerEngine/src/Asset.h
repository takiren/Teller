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

	class Episode{
	private:
		std::string title;
		int number;
		int line_begin;
		int line_end;
	public:
		std::map<int, std::vector<std::string>> data;
		Episode() :
			title("Nothing title"),
			number(0),
			line_begin(0),
			line_end(0)
		{};
		Episode(std::string titleText, int episode_num) :
			title(titleText),
			number(episode_num),
			line_begin(0),
			line_end(0)
		{};

		Episode(std::map<int, std::vector<std::string>> csv) :
			data(csv),
			title("Nothing title"),
			number(0),
			line_begin(0),
			line_end(0)
		{};

		~Episode() = default;
		void SetLineBegin(int line);
		void SetLineEnd(int line);
		void SetNumber(int episodeNumber);

	};

	/*
	すべてのアセットはコンテンツマネージャーが管理し、
	エージェントやその他のUIは
	コンテンツマネージャーを介してデータを取得する。
	*/

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

}
