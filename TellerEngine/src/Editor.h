#pragma once
#include<Windows.h>
#include<memory>
#include<string>
#include<vector>
#include<iostream>
#include<cinder/Cinder.h>
#include<cinder/CinderImGui.h>
#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include<cinder/ImageIo.h>
#include<imgui_node_editor.h>
#include <cinder/Rand.h>
#include<cinder/Log.h>
#include<uuids.h>

#include <utilities/builders.h>
#include <utilities/widgets.h>
#include<jsoncpp/json.h>

#include<Core.h>

#include"TellerCore.h"
#include"ContentManager.h"
#include"Episode.h"
#include"TellerEvent.h"
#include"Tree.h"

namespace Teller {
	class TellerCore;
	using CSVManager = ContentsManager<CSVLoader>;
	using EpisodeManager = ContentsManager<Episode>;

	using WeakEpisodeCM = std::weak_ptr<EpisodeManager>;
	using WeakCSVCM = std::weak_ptr<CSVManager>;

	namespace ed = ax::NodeEditor;
	using namespace ax;
	using ax::Widgets::IconType;

	class EditorManager :public std::enable_shared_from_this<EditorManager> {

	};

	enum class EDITOR_TYPE
	{
		NODE_EDITOR,
		EPISODE_EDITOR
	};

	class Editor :public std::enable_shared_from_this<Editor> {
	protected:
		bool bEnabled;
	public:
		std::weak_ptr<TellerCore> parent;

		Editor() :
			bEnabled(true)
		{};
		virtual void Tick();
		void TickInternal();
		virtual ~Editor() = default;
		virtual void Update();

		virtual void Save();

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
		std::weak_ptr<CSVManager> ptr_csvContentManger;
		std::weak_ptr<EpisodeManager> ptr_episodeContentManager;
		//エピソードCM

		std::pair<int, int> lineBracket;

		std::vector<uint64_t> fileVec_;
		std::map<int, std::vector<std::string>> data;
		std::string episodeNameCandidate;
		void Initialize();
	public:
		EpisodeEditor() :
			Editor(),
			lineBracket(std::make_pair<int, int>(0, 1)),
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

	class EpisodeEventEditor :
		public Editor
	{
	private:

		ImColor GetIconColor(Socket_TYPE type);
		void DrawPinIcon(const std::shared_ptr<TSocketCore> sckt, bool connected, int alpha)
		{
			IconType iconType;
			ImColor  color = GetIconColor(sckt->type_);
			color.Value.w = alpha / 255.0f;

			switch (sckt->type_)
			{
			case  Teller::Socket_TYPE::FLOW:		iconType = IconType::Flow;   break;
			case Teller::Socket_TYPE::BOOL:			iconType = IconType::Circle; break;
			case Teller::Socket_TYPE::INT:			iconType = IconType::Circle; break;
			case Teller::Socket_TYPE::OPTION:		iconType = IconType::Circle; break;
			default:
				return;
			}

			ax::Widgets::Icon(ImVec2(s_PinIconSize, s_PinIconSize), iconType, connected, color, ImColor(32, 32, 32, alpha));
		};

		struct LinkInfo {
			ed::LinkId Id;
			ed::PinId  InputId;
			ed::PinId  OutputId;
		};

		void ShowLeftPane(float panewidth);

		bool g_FirstFrame = true;

		void OpenAddNodePopup();

		ImVector<LinkInfo> g_Links;
		ed::EditorContext* gContext;
		std::weak_ptr<EpisodeManager> ptrEpsdMngr;
		const int s_PinIconSize = 24;

		uint64_t currentEpisodeID_;

		std::unique_ptr<TNodeManager> ptrTNodeManager;

		bool bShiftDown;

		std::vector<std::string> nodeList_;
	public:
		EpisodeEventEditor() :
			Editor(),
			ptrTNodeManager(std::move(std::make_unique<TNodeManager>())),
			gContext(ed::CreateEditor()),
			bShiftDown(false),
			currentEpisodeID_(0)
		{
			nodeList_.push_back("Branch.");
			nodeList_.push_back("Scene change");
			nodeList_.push_back("Animation.");
			nodeList_.push_back("Episode.");
			nodeList_.push_back("Event.");
			nodeList_.push_back("Comment");
		};

		~EpisodeEventEditor() = default;
		void Tick() override;
		void Update() override;
		void CallByParent() override;
	};


	class NodeEditorBase :public Editor {
	private:
		std::string name;
	public:
		NodeEditorBase() = default;
		NodeEditorBase(std::string _name);
		virtual void Tick();
	};

	class SequenceEditor :public Editor {
	private:

		ImColor GetIconColor(Socket_TYPE type);
		void DrawPinIcon(const std::shared_ptr<TSocketCore> sckt, bool connected, int alpha)
		{
			IconType iconType;
			ImColor  color = GetIconColor(sckt->type_);
			color.Value.w = alpha / 255.0f;

			switch (sckt->type_)
			{
			case  Teller::Socket_TYPE::FLOW:		iconType = IconType::Flow;   break;
			case Teller::Socket_TYPE::BOOL:			iconType = IconType::Circle; break;
			case Teller::Socket_TYPE::INT:			iconType = IconType::Circle; break;
			case Teller::Socket_TYPE::OPTION:		iconType = IconType::Circle; break;
			default:
				return;
			}

			ax::Widgets::Icon(ImVec2(s_PinIconSize, s_PinIconSize), iconType, connected, color, ImColor(32, 32, 32, alpha));
		};

		struct LinkInfo {
			ed::LinkId Id;
			ed::PinId  InputId;
			ed::PinId  OutputId;
		};

		const int s_PinIconSize = 24;
		std::string name;
		// EpisodeManagerへのポインタ。
		std::weak_ptr<EpisodeManager> ptrEPCM;
		std::unique_ptr<TNodeManager> ptrTNodeManager;
		void UpdateEpisodeList();

		std::map<uint64_t, std::string> episodeMap;
		ed::EditorContext* gContext;
		void Initialize();
	public:
		SequenceEditor() :
			name("SequenceEditor"),
			Editor(),
			ptrTNodeManager(std::make_unique<TNodeManager>()),
			gContext(ed::CreateEditor())
		{
			Initialize();
		};

		~SequenceEditor() = default;

		void Tick() override;
		void callBackFromCSVManager(std::vector < std::string> _episode);
		void CallByParent() override;
	};

	class AssetViewer :public Editor {
	private:
	public:
		AssetViewer() :Editor() {};
		void Tick() override;
	};

	class CharacterEditor :public Editor {
	private:
		std::vector<fs::path> imgPaths_;
		std::vector<fs::path> dirPaths_;
		void Initialize(fs::path _path);
		fs::path OpenFile();
	public:
		CharacterEditor() :Editor() {
			fs::path p = fs::current_path();
			p = p.parent_path();
			p /= fs::path("data\\images");
			Initialize(p);
		};
		CharacterEditor(fs::path _path) :Editor() {
			Initialize(_path);
		};

		void Tick()override;

	};
}