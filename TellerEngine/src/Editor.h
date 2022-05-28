#pragma once

#include<memory>
#include<string>
#include<vector>

#include"cinder/Cinder.h"
#include"cinder/CinderImGui.h"


#include"TellerCore.h"
#include"ContentManager.h"
#include"Episode.h"
#include"TellerEvent.h"

namespace Teller {
	class TellerCore;

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
		virtual ~Editor() = default;
		virtual void Tick();
		virtual void Update();

		//コピー禁止
		Editor(const Editor&) = delete;
		Editor& operator=(const Editor&) = delete;
		//ムーブ許可
		Editor& operator=(Editor&&) = default;
		virtual void AttachEvent(CALL_BACK_EVENT _event);
	};

	class TopLevelMenu :public Editor {
	private:
	public:
		TopLevelMenu() : Editor() {};
		void Tick() override;
	};


	class EpisodeEditor :public Editor {
	private:
		//読み込まれた生のCSVファイルリスト
		std::vector<std::string> loadedCsvFiles;

		//コンテンツマネージャーへのポインタ。
		std::weak_ptr<ContentsManager<CSVLoader>> ptr_csvContentManger;
		std::weak_ptr<std::map<
			std::string,
			std::shared_ptr<CSVLoader>>> csv_contents;
	public:
		EpisodeEditor() :Editor() {};
		~EpisodeEditor() = default;
		void Tick() override;

		void CB_CSV_Update();
	};

	class AssetViewer :public Editor {
	private:
	public:
		AssetViewer() :Editor() {};
		void Tick() override;
	};
}