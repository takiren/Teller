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
#include"NodeEditor.h"


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
	namespace util = ax::NodeEditor::Utilities;
	using ax::Widgets::IconType;

	//TODO:Delete
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
	private:
	protected:
		static std::weak_ptr<TellerCore> owner;
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

		//メッセージ用
		virtual void GetTMessage(TEVENT_MESSAGE& _message) {};

		virtual void CallByParent();

		//ロード
		virtual void LoadFile(fs::path _path);

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

	class EpisodeEventNodeEditor final :public NodeEditorBase<Episode_Event_Node, Socket_Data_Type> {
	private:
	protected:
		void Initialize() override;

		void DrawInputSocketInternal() override;
		void DrawOutputSocketInternal() override;

	public:
		EpisodeEventNodeEditor() :
			NodeEditorBase("EventNodeEditor")
		{};
		~EpisodeEventNodeEditor() = default;

		void LoadFile(fs::path _path) override;

		void AddNodeSignature(TNodeSignature<Socket_Data_Type> _nodesig) override;
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

		void LoadFile(fs::path _path) override;
	};

	//エピソードイベントエディター
	class EpisodeEventEditor final :public Editor {
	private:
		std::unique_ptr<NodeEditorBase<Episode_Event_Node, Socket_Data_Type>> tnodeEditor;

		TEposodeID									currentEpisodeID_;

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

		//プレビューするかどうか
		bool				bShowSprite;

		utils::UIDGenerator uidgen;

		void UpdateAssetList();
		void DrawLinks();

		void NodeEditorTick();

	protected:
	public:
		EpisodeEventEditor() :
			Editor("EpisodeEventEditor"),

			currentEpisodeID_(0),
			episodeRef(nullptr),
			jsonEpisode(nullptr),
			currentLine(0),
			characterAppearanceNum(0),
			eventPackNum(0),
			bShowSprite(true),
			previewText(std::make_unique<MainTextArea>()),
			previewTextChanger(std::make_unique<TextChanger>()),
			uidgen(utils::UIDGenerator()),
			tnodeEditor(std::make_unique< NodeEditorBase<Episode_Event_Node, Socket_Data_Type>>())
		{
			//HACK:なんでこんな雑な感じなの？
			//ノード用

			previewTextChanger->AttachToAgent(previewText);
		}
		~EpisodeEventEditor() = default;

		//TODO:イベントを外部から登録できるようにしたい
		void Tick() override;
		void Update() override;

		void CallByParent() override;

		void LoadFile(fs::path _path) override;

		//TODO:早くどうにかしろ
		void Save() override;
	};


	class TestEditor :public Editor {
	private:
		using TNodeEditor = NodeEditorBase<Episode_Event_Node, Socket_Data_Type>;
		std::unique_ptr<TNodeEditor> nodeEditorRef;

	public:
		TestEditor() :
			Editor("TestEditor"),
			nodeEditorRef(std::make_unique<TNodeEditor>("NodeTest"))
		{
			TNodeSignature<Socket_Data_Type> nsig{ "Branch" };

			nsig.inputSockets.push_back(
				std::make_pair<Socket_Data_Type, IconType>(Socket_Data_Type::FLOW, IconType::Circle)
			);

			nsig.outputSockets.push_back(
				std::make_pair<Socket_Data_Type, IconType>(Socket_Data_Type::FLOW, IconType::Circle)
			);

			nsig.outputSockets.push_back(
				std::make_pair<Socket_Data_Type, IconType>(Socket_Data_Type::FLOW, IconType::Circle)
			);

			nodeEditorRef->AddNodeSignature(nsig);
		};

		void Tick() override {
			ImGui::Begin("TestEditor");
			ImGui::BeginChild("testNodeed");

			nodeEditorRef->Tick();

			ImGui::EndChild();
			ImGui::End();
		};
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