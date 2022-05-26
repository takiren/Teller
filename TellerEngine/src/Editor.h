#pragma once
#include"Core.h"
#include"ModuleCore.h"
#include"Episode.h"
#include"ContentManager.h"
#include"TellerCore.h"

#include<string>
#include<vector>
#include<map>
#include<memory>

namespace Teller {
	class TellerCore;
	template<class TYPE>
	class ContentManager;
	class CSVLoader;

	enum class EDITOR_TYPE
	{
		NODE_EDITOR,
		EPISODE_EDITOR
	};

	class Editor :public std::enable_shared_from_this<Editor> {
	private:
	protected:
	public:
		std::weak_ptr<TellerCore> parent;

		Editor() = default;
		virtual void Tick();
		virtual void Update();

		//コピー禁止
		Editor(const Editor&) = delete;
		Editor& operator=(const Editor&) = delete;
		//ムーブ許可
		Editor& operator=(Editor&&) = default;
	};

	class TopLevelMenu :public Editor {
	private:
	public:
		TopLevelMenu() : Editor() {};
		void Tick() override;
	};

	class EpisodeEditor :public Editor {
	private:
		std::vector<std::string> loadedCsvFiles;
		std::weak_ptr<ContentManager<CSVLoader>> ptr_csvContentManger;
		void UpdateKeys();
	public:
		EpisodeEditor() :
			Editor(),
			ptr_csvContentManger(parent.lock()->GetCSVContentsManager())
		{
			loadedCsvFiles = ptr_csvContentManger.lock()->GetKeys();
		};

		void Tick() override;
	};

	class AssetViewer :public Editor {
	private:
	public:
		AssetViewer() :Editor() {};
		void Tick() override;
	};
}