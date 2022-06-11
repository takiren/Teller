#pragma once
#include<Windows.h>
#include<memory>
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<istream>
#include<functional>

#include<cinder/Cinder.h>
#include<cinder/CinderImGui.h>
#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include<cinder/ImageIo.h>
#include <cinder/Rand.h>
#include<cinder/Log.h>
#include<uuids.h>
#include<cppglob/glob.hpp>

#include <utilities/builders.h>
#include <utilities/widgets.h>
#include<imgui_node_editor.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include<imgui_internal.h>
#include<jsoncpp/json.h>

#include<Core.h>

#include"TellerCore.h"
#include"ContentManager.h"
#include"Episode.h"
#include"TellerEvent.h"
#include"Tree.h"
#include"NodeLink.h"

namespace Teller {
	class TellerCore;

	using CSVManager = ContentsManager<CSVLoader>;
	using EpisodeManager = ContentsManager<Episode>;
	using WeakEpisodeCM = std::weak_ptr<EpisodeManager>;
	using WeakCSVCM = std::weak_ptr<CSVManager>;

	//エイリアス
	namespace ed = ax::NodeEditor;
	namespace fs = std::filesystem;
	using namespace ax;
	using ax::Widgets::IconType;

	//エディターを管理するクラスがあればいいのかなって思ったけどいらないからいずれ削除
	class EditorManager :public std::enable_shared_from_this<EditorManager> {
		//TellerCoreへのweak ptr
		std::weak_ptr<TellerCore> parent;
	public:
		EditorManager() {};
		virtual ~EditorManager() = default;
		void EditorCallBack(std::string _filename);
		void EditorCallBack(std::unique_ptr<Episode> _episode);
	};

	//多分いらない
	enum class EDITOR_TYPE
	{
		NODE_EDITOR,
		EPISODE_EDITOR
	};

	class Editor :public std::enable_shared_from_this<Editor> {
	protected:
		bool bEnabled;
	public:

		Editor() :name_(""), bEnabled(true) {};
		Editor(std::string _name) :
			bEnabled(true),
			name_(_name) {};

		virtual ~Editor() = default;

		std::weak_ptr<TellerCore> parent;
		std::string name_;

		//コピー禁止
		Editor(const Editor&) = delete;
		Editor& operator=(const Editor&) = delete;
		//ムーブ許可
		Editor& operator=(Editor&&) = default;

		//depricated
		virtual void TickInternal();
		virtual void Tick();
		virtual void Update();

		virtual void Save();

		virtual bool CanAccept(fs::path _path) = 0;
		virtual void GetTMessage(TEVENT_MESSAGE& _message) {};

		virtual void CallByParent();
		virtual void LoadFile(fs::path _path) = 0;

		//参照 : imgui-node-editor->Splitter
		virtual bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f);
	};

	//メニューバー用
	class TopLevelMenu :public Editor {
	private:
	public:
		TopLevelMenu() : Editor("TopLevelMenu") {};
		void Tick() override;
		void LoadFile(fs::path _path) override;
	};

	//csvからエピソードファイルを作成するためのエディター
	class EpisodeEditor :public Editor {
	private:
		//読み込まれた生のCSVファイルリスト
		//std::vector<std::string> loadedCsvFiles;

		//コンテンツマネージャーへのポインタ。
		std::weak_ptr<CSVManager> csvContentMangerRef;
		std::weak_ptr<EpisodeManager> episodeContentManagerRef;
		//エピソードCM

		std::pair<int, int> lineBracket;

		//ファイルリスト
		std::vector<uint64_t> fileVec_;
		std::map<int, std::vector<std::string>> data;
		std::string episodeNameCandidate;

		//初期化用関数。
		void Initialize();
	public:
		EpisodeEditor() :
			Editor("EpisodeEditor"),
			lineBracket(std::make_pair<int, int>(0, 1)),
			episodeNameCandidate("") {};

		~EpisodeEditor() = default;

		void CallByParent() override;

		void Tick() override;

		std::string SingleLine(std::vector<std::string > _vector);

		bool CanAccept(fs::path _path) override;
		void LoadFile(fs::path _path) override;
	};

	//エピソードイベントエディター
	class EpisodeEventEditor :public Editor {
	private:
		//ノードエディタ用変数
		ImColor GetIconColor(Socket_TYPE type);
		void DrawPinIcon(const std::shared_ptr<TSocketCore> sckt, bool connected, int alpha);

		//左のパネルを表示
		void ShowLeftPane(float panewidth);
		bool g_FirstFrame = true;

		void OpenAddNodePopup();

		//ノードエディタ用変数
		ImVector<LinkInfo> g_Links;
		ed::EditorContext* gContext;

		//deprecated　これ必要ない。
		std::weak_ptr<EpisodeManager> EpsdMngrRef;

		int s_PinIconSize;
		uint64_t currentEpisodeID_;

		std::unique_ptr<TNodeManager> TNodeManagerRef;

		//まじでいらない
		bool bShiftDown;

		std::vector<std::string> nodeList_;
		std::unique_ptr<Episode> episodeRef;

		int currentLine;

		json jsonEpisode;
		std::vector<json> jsonCharacter; //多分いらない。
		std::unordered_map<std::string, json> jsonCharacterMap;

		void Initialize();
		void LoadEpisode(fs::path _path);
		void LoadCharacterJson(fs::path _path);
	public:
		EpisodeEventEditor() :
			Editor("EpisodeEventEditor"),
			TNodeManagerRef(std::move(std::make_unique<TNodeManager>())),
			gContext(ed::CreateEditor()),
			bShiftDown(false),
			currentEpisodeID_(0),
			episodeRef(nullptr),
			jsonEpisode(nullptr),
			currentLine(0),
			s_PinIconSize(24)
		{
			nodeList_.push_back("Branch.");
			nodeList_.push_back("Scene change");
			nodeList_.push_back("Animation.");
			nodeList_.push_back("Episode.");
			nodeList_.push_back("Event.");
			nodeList_.push_back("Comment");
			nodeList_.push_back("Character In Out");
		};

		~EpisodeEventEditor() = default;
		void Tick() override;
		void Update() override;
		void CallByParent() override;

		void UpdateAssetList();

		void AttachEpisode(std::shared_ptr<Episode> _episode);
		bool CanAccept(fs::path _path) override;

		void SetEpisode();
		void LoadFile(fs::path _path) override;
	};

	//ノードエディタ用基底クラス
	//はやくこれに基底クラスを移さないとこれからが大変。
	class NodeEditorBase :public Editor {
	public:
		NodeEditorBase() = delete;
		NodeEditorBase(std::string _name) : Editor("Node Editor Base") {};
		virtual void Tick();
		void LoadFile(fs::path _path) override;
	};


	class SequenceEditor :public Editor {
	private:
		ImColor GetIconColor(Socket_TYPE type);

		int s_PinIconSize;
		// EpisodeManagerへのポインタ。
		std::weak_ptr<EpisodeManager> ptrEPCM;
		std::unique_ptr<TNodeManager> TNodeManagerRef;

		std::map<uint64_t, std::string> episodeMap;
		ed::EditorContext* gContext;

		void Initialize();
		void UpdateEpisodeList();
		void DrawPinIcon(const std::shared_ptr<TSocketCore> sckt, bool connected, int alpha);

	public:
		SequenceEditor() :
			Editor("SequenceEditor"),
			TNodeManagerRef(std::make_unique<TNodeManager>()),
			gContext(ed::CreateEditor()),
			s_PinIconSize(24)
		{
			Initialize();
		};
		~SequenceEditor() = default;

		void Tick() override;
		void callBackFromCSVManager(std::vector < std::string> _episode);

		void LoadFile(fs::path _path) override;

	};

	class AssetViewer :public Editor {
	private:
		std::weak_ptr<EpisodeManager> episodeMgrRef;

		//アセットへのパス
		fs::path episodePath_;
		fs::path characterPath_;
		void Initialize();

		//std::unordered_map<fs::path, std::unique_ptr<Editor>> editorsRef;

		//TellerCoreのLoadFileToEditorのコールバック。
	protected:
	public:
		std::function<void(fs::path)> TCcallback_;

		AssetViewer() :Editor("Asset Viewer") { Initialize(); };

		void Tick() override;

		bool CanAccept(fs::path _path) override;
		void LoadFile(fs::path _path) override;

		void CallByParent() override;
	};

	class CharacterEditor :public Editor {
	private:
		std::vector<fs::path> imgPaths_;
		std::vector<fs::path> dirPaths_;
		void Initialize(fs::path _path);
		fs::path OpenFile();
	public:
		CharacterEditor() :Editor("Character Editor") {
			fs::path p = fs::current_path();
			p = p.parent_path();
			p /= fs::path("data\\images");
			Initialize(p);
		};
		CharacterEditor(fs::path _path) :Editor("Character Editor") {
			Initialize(_path);
		};

		void Tick()override;
		void LoadFile(fs::path _path) override;
	};
}