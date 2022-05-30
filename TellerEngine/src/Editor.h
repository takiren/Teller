#pragma once
#include<memory>
#include<string>
#include<vector>
#include<iostream>
#include <stdio.h>
#include<cinder/Cinder.h>
#include<cinder/CinderImGui.h>

#include<Core.h>

#include"TellerCore.h"
#include"ContentManager.h"
#include"Episode.h"
#include"TellerEvent.h"

namespace Teller {
	class TellerCore;
	using CSVManager = ContentsManager<CSVLoader>;
	using EpisodeManager = ContentsManager<Episode>;

	class EditorManager :public std::enable_shared_from_this<EditorManager> {

	};

	enum class EDITOR_TYPE
	{
		NODE_EDITOR,
		EPISODE_EDITOR
	};

	class Editor :public std::enable_shared_from_this<Editor> {
	private:
		std::pair<int, int> lineBracket;
	protected:
		bool bEnabled;
	public:
		std::weak_ptr<TellerCore> parent;

		Editor() = default;
		virtual void Tick();
		void TickInternal();
		virtual ~Editor() = default;
		virtual void Update();

		//コピー禁止
		Editor(const Editor&) = delete;
		Editor& operator=(const Editor&) = delete;
		//ムーブ許可
		Editor& operator=(Editor&&) = default;

		void GetMessage(TEVENT_MESSAGE& _message) {

		}
		virtual void CallByParent();

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
		//std::vector<std::string> loadedCsvFiles;

		//コンテンツマネージャーへのポインタ。
		std::pair<int, int> lineBracket;

		std::weak_ptr<CSVManager> ptr_csvContentManger;
		std::vector<std::string> fileVec_;
		std::map<int, std::vector<std::string>> data;

		std::string episodeNameCandidate;
		void Initialize();
	public:
		EpisodeEditor() :
			Editor(),
			lineBracket(std::make_pair<int, int>(0, 0)),
			episodeNameCandidate("")
		{
			Initialize();
		};

		~EpisodeEditor() = default;

		void CallByParent() override;

		void Tick() override;

		std::string SingleLine(std::vector<std::string > _vector) {
			auto s = std::string("");
			for (auto& e : _vector)s += e;
			return s;
		}
	};

	class EpisodeEventEditor :public Editor {
	private:
		std::weak_ptr<EpisodeManager> ptrEpsdMngr;
	public:
		EpisodeEventEditor() :
			Editor()
		{};
		void Tick() override;
		void Update() override;
		void CallByParent() override;

	};

	class AssetViewer :public Editor {
	private:
	public:
		AssetViewer() :Editor() {};
		void Tick() override;
	};
}