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
#include"Animation.h"
#include"NodeLink.h"
#include"utility.h"


namespace teller {
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
		//trueのときtickを呼び出す。
		bool bEnabled;

		fs::path openedFilePath_;
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

		//毎チック呼ばれる処理
		virtual void Tick();

		//おそらく必要ない
		virtual void Update();

		//エディターでの編集を保存する
		virtual void Save();

		//いらない
		virtual bool CanAccept(fs::path _path) = 0;

		//メッセージ用
		virtual void GetTMessage(TEVENT_MESSAGE& _message) {};

		virtual void CallByParent();

		//ロード
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
		std::pair<int, int> lineBracket;

		//ファイルリスト
		std::string episodeNameCandidate;

		std::unique_ptr<CSVLoader> CSVRef;

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
	class EpisodeEventEditor final :public Editor {
	private:
		//ノードエディタ用変数
		ImColor GetIconColor(Socket_TYPE type);
		void DrawPinIcon(const std::shared_ptr<TSocketCore> sckt, bool connected, int alpha);
		int											s_PinIconSize;

		//左のパネルを表示
		void ShowLeftPane(float panewidth);
		bool										g_FirstFrame = true;

		void OpenAddNodePopup();

		//ノードエディタ用変数
		ed::EditorContext* gContext;
		std::unique_ptr<TNodeManager>				TNodeManagerRef;

		TEposodeID									currentEpisodeID_;

		//追加できるノードの一覧
		std::vector<std::string>					nodeList_; //HACK:なんでvectorなのにList_なの？
		std::unique_ptr<Episode>					episodeRef;

		//編集対象の行
		int											currentLine;

		//キャラクター見た目
		int											characterAppearanceNum;
		//EventPack編集行
		int											eventPackNum;

		//json
		json										jsonEpisode;
		std::unordered_map<std::string, json>		jsonCharacterMap;
		std::unordered_map<std::string, fs::path>	characterPathMap;
		fs::path									jsonFilePath_;

		using EventPack = std::vector<std::unique_ptr<EpisodeEvent>>;
		std::map<int, EventPack>					eventPackMap;

		void CreateEpisodeEvent(EPISODE_EVENT_TYPE _type, int _line, std::string _target, std::string _key);
		void SwapEvent(EventPack& _vector, int m, int n);
		void Initialize();
		void LoadEpisode(fs::path _path);
		void LoadCharacterJson(fs::path _path);

		//TODO:実装しろ
		void LoadEpisodeEvent(json _j);

		//プレビュー用
		void ShowPreview();
		void ShowEpisodeText();

		std::unordered_map<std::string, std::shared_ptr<CharacterSimple>>				previewCharacterMap;
		std::unordered_map<std::string, std::unique_ptr<CharacterAppearanceChanger>>	previewAnimationMap;
		std::shared_ptr<MainTextArea>													previewText;
		std::unique_ptr<TextChanger>													previewTextChanger;

		//Character.jsonのfile情報を取得
		std::vector<std::string> GetSpritesName(json _cjson);
		std::string targetCharacter = "";

		bool				bCreatingNewNode;
		//プレビューするかどうか
		bool				bShowSprite;

		utils::UIDGenerator uidgen;

		void UpdateAssetList();
		void DrawLinks();

	protected:
	public:
		EpisodeEventEditor() :
			Editor("EpisodeEventEditor"),
			TNodeManagerRef(std::make_unique<TNodeManager>()),
			gContext(ed::CreateEditor()),
			currentEpisodeID_(0),
			episodeRef(nullptr),
			jsonEpisode(nullptr),
			currentLine(0),
			s_PinIconSize(24),
			characterAppearanceNum(0),
			eventPackNum(0),
			bShowSprite(true),
			previewText(std::make_unique<MainTextArea>()),
			previewTextChanger(std::make_unique<TextChanger>()),
			bCreatingNewNode(false),
			uidgen(utils::UIDGenerator())
		{
			//HACK:なんでこんな雑な感じなの？
			//ノード用
			nodeList_.push_back("Branch.");
			nodeList_.push_back("Scene change");
			nodeList_.push_back("Animation.");
			nodeList_.push_back("Episode.");
			nodeList_.push_back("Event.");
			nodeList_.push_back("Comment");
			nodeList_.push_back("Character In Out");

			previewTextChanger->AttachToAgent(previewText);
		}
		~EpisodeEventEditor() = default;

		//TODO:イベントを外部から登録できるようにしたい
		void Tick() override;
		void Update() override;

		void CallByParent() override;

		//TODO: Delete
		bool CanAccept(fs::path _path) override;

		void LoadFile(fs::path _path) override;

		//TODO:早くどうにかしろ
		void Save() override;
	};


	//ノードエディタ用基底クラス
	//TODO:こいつを早く実装するか削除しろ
	class NodeEditorBase {
	private:
	protected:
		std::string								name_;
		int										s_PinIconSize = 24;

		ed::EditorContext*						gContext;
		std::unique_ptr<TNodeManager>			TNodeManagerRef;

		//追加できるノードの一覧

		utils::UIDGenerator						uidgen;
		std::unordered_map<std::string, TNodeSignature> nodeSignatureVector;

		bool									bCreatingNewNode;

		void	DrawPinIcon(const std::shared_ptr<TSocketCore> sckt, bool connected, int alpha);
		ImColor GetIconColor(Socket_TYPE _type);

		void OpenPopupAddNode();

		TNodeID MakeNode(int _index);
	public:
		NodeEditorBase() = delete;
		NodeEditorBase(std::string _name) :
			name_(_name),
			uidgen(utils::UIDGenerator()),
			gContext(ed::CreateEditor()),
			bCreatingNewNode(false)
		{};
		virtual ~NodeEditorBase() = default;

		//コピーコンストラクタ削除
		NodeEditorBase(const NodeEditorBase&) =				delete;
		NodeEditorBase& operator=(const NodeEditorBase&) =	delete;
		//ムーブ許可
		NodeEditorBase& operator=(NodeEditorBase&&) =		default;

		virtual void LoadFile(fs::path _path);
		virtual void Tick();

		virtual void AddNodeSignature(TNodeSignature _nodeSig);
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

		fs::path targetFile;

		fs::path dataPath_;

	protected:
	public:
		//TODO:DELETE
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