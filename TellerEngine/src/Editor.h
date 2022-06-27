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
	namespace util = ax::NodeEditor::Utilities;
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
	private:
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
		//ノードエディタ用変数
		ImColor GetIconColor(Socket_Data_Type type);
		void DrawPinIcon(const std::shared_ptr<TSocketCore<Episode_Event_Node, Socket_Data_Type>> sckt, bool connected, int alpha);
		int											s_PinIconSize;

		//左のパネルを表示
		void ShowLeftPane(float panewidth);
		bool										g_FirstFrame = true;

		void OpenAddNodePopup();

		//ノードエディタ用変数
		ed::EditorContext* gContext;
		std::unique_ptr<TNodeManager<Episode_Event_Node, Socket_Data_Type>>				TNodeManagerRef;

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
			TNodeManagerRef(std::make_unique<TNodeManager<Episode_Event_Node, Socket_Data_Type>>()),
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

		void LoadFile(fs::path _path) override;

		//TODO:早くどうにかしろ
		void Save() override;
	};

	//ノードエディタ用基底クラス
	template<class NODE_TYPE, class SOCKET_TYPE>
	class NodeEditorBase {
	private:
	protected:
		std::string								name_;
		int										s_PinIconSize = 24;

		ed::EditorContext* gContext;
		std::unique_ptr<TNodeManager<NODE_TYPE, SOCKET_TYPE>>			TNodeManagerRef;

		utils::UIDGenerator						uidgen;
		//追加できるノードの一覧
		std::vector<TNodeSignature<SOCKET_TYPE>>				nodeSignatureVector;

		bool									bCreatingNewNode;

		void	DrawPinIcon(const std::shared_ptr<TSocketCore<NODE_TYPE, SOCKET_TYPE>> sckt, bool connected, int alpha);
		ImColor GetIconColor(TSocketCore<NODE_TYPE, SOCKET_TYPE>& _type);

		//Opens a Popup to add Node.
		void	OpenPopupAddNode();

		//It returns TNodeID
		TNodeID MakeNode(int _index);

		virtual void Initialize();
	public:
		NodeEditorBase() = delete;
		NodeEditorBase(std::string _name) :
			name_(_name),
			uidgen(utils::UIDGenerator()),
			gContext(ed::CreateEditor()),
			bCreatingNewNode(false),
			TNodeManagerRef(std::make_unique<TNodeManager<Episode_Event_Node, Socket_Data_Type>>())
		{
			Initialize();
		};
		virtual ~NodeEditorBase() = default;

		//コピーコンストラクタ削除
		NodeEditorBase(const NodeEditorBase&) = delete;
		NodeEditorBase& operator=(const NodeEditorBase&) = delete;
		//ムーブ許可
		NodeEditorBase& operator=(NodeEditorBase&&) = default;

		virtual void LoadFile(fs::path _path);
		virtual void Tick();

		virtual void AddNodeSignature(TNodeSignature<SOCKET_TYPE> _nodeSig);
	};

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline ImColor teller::NodeEditorBase< NODE_TYPE, SOCKET_TYPE>::GetIconColor(TSocketCore<NODE_TYPE, SOCKET_TYPE>& socket)
	{
		switch (socket.icontype_)
		{
		case IconType::Flow:			return ImColor(255, 255, 255);
		case IconType::Circle:			return ImColor(220, 48, 48);
		case IconType::Square:			return ImColor(68, 201, 156);
		case IconType::Grid:			return ImColor(147, 226, 74);
		case IconType::RoundSquare:		return ImColor(255, 255, 255);
		case IconType::Diamond:			return ImColor(125, 125, 125);
		default:						return ImColor(0, 0, 0);
		}
	}
	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void teller::NodeEditorBase< NODE_TYPE, SOCKET_TYPE>::OpenPopupAddNode()
	{
		std::string popname = name_;
		popname += "AddNode";
	
		ed::Suspend();
		if (ed::ShowBackgroundContextMenu()) {
			ImGui::OpenPopup(popname.c_str());
		}
		ed::Resume();

		ed::Suspend();

		if (ImGui::BeginPopup(popname.c_str())) {

			ImGui::Text("Node list.");
			ImGui::Separator();

			int i = 0;
			for (auto& nodesig : nodeSignatureVector)
				if (ImGui::Selectable(nodesig.name.c_str()))
					ed::SetNodePosition(MakeNode(i),ImGui::GetMousePos());

			++i;

			ImGui::EndPopup();
		}
		ed::Resume();
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::MakeNode(int _index)
	{
		auto nid = TNodeManagerRef->AddNodeFromSignature(nodeSignatureVector[_index]);

		return nid;
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void teller::NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::Initialize()
	{
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void teller::NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::DrawPinIcon(const std::shared_ptr<TSocketCore<NODE_TYPE, SOCKET_TYPE>> sckt, bool connected, int alpha)
	{
		ImColor  color = GetIconColor(*sckt.get());
		color.Value.w = alpha / 255.0f;

		/*
		switch (sckt->type_)
		{
		case teller::Socket_Data_Type::FLOW:		iconType = IconType::Flow;   break;
		case teller::Socket_Data_Type::BOOL:		iconType = IconType::Circle; break;
		case teller::Socket_Data_Type::INT:			iconType = IconType::Circle; break;
		case teller::Socket_Data_Type::OPTION:		iconType = IconType::Circle; break;
		case teller::Socket_Data_Type::STRING:		iconType = IconType::Circle; break;
		default:
			return;
		}
		*/

		ax::Widgets::Icon(ImVec2(s_PinIconSize, s_PinIconSize), sckt->icontype_, connected, color, ImColor(32, 32, 32, alpha));
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void teller::NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::Tick()
	{
		ed::SetCurrentEditor(this->gContext);
		ed::Begin(name_.c_str());

		OpenPopupAddNode();

		//1.ノードマネージャーから読み取って描画
		{
			typename util::BlueprintNodeBuilder builder;
			// ノードでイテレーション
			for (auto& node : TNodeManagerRef->nodes) {
				builder.Begin(node.second->ID_);
				//builderでの操作。
				{
					builder.Header(ImColor(255, 255, 255));
					{
						ImGui::Spring(0);
						ImGui::TextUnformatted(node.second->name_.c_str());
						ImGui::Spring(1);
						ImGui::Dummy(ImVec2(0, 28));
						ImGui::Spring(0);
					}
					builder.EndHeader();

					// インプットソケットの描画
					{
						auto alpha = ImGui::GetStyle().Alpha;
						for (auto& e : node.second->socketsInput) {

							builder.Input(e.first);
							ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
							DrawPinIcon(e.second, false, (int)(alpha * 255));
							ImGui::Spring(0);
							ImGui::Spring(0);
							ImGui::PopStyleVar();
							builder.EndInput();
						}
					}

					// アウトプットソケットの描画
					{
						auto alpha = ImGui::GetStyle().Alpha;
						for (auto& e : node.second->socketsOutput) {
							ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
							builder.Output(e.first);
							ImGui::Spring(0);
							ImGui::TextUnformatted(node.second->name_.c_str());
							// ピンごとの条件分岐を記述ここから

							// ここまで
							ImGui::Spring(0);
							DrawPinIcon(e.second, false, (int)(alpha * 255));
							ImGui::PopStyleVar();
							builder.EndOutput();
						}
					}
				}
				builder.End();
			}
		}
		//2.リンク描画
		{
			auto links = TNodeManagerRef->GetLinks();
			for (auto& link : links)
				ed::Link(link.ID_, link.InputID_, link.OutputID_);
		}

		//3.リンク生成
		{
			//ノードを生成していないときに処理
			if (!bCreatingNewNode) {
				if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f)) {
					auto showLabel = [](const char* label, ImColor _color) {
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
						auto size = ImGui::CalcTextSize(label);

						auto padding = ImGui::GetStyle().FramePadding;
						auto spacing = ImGui::GetStyle().ItemSpacing;

						ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

						auto rectMin = ImGui::GetCursorScreenPos() - padding;
						auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

						auto drawlist = ImGui::GetWindowDrawList();
						drawlist->AddRectFilled(rectMin, rectMax, _color, size.y * 0.15f);

						ImGui::TextUnformatted(label);
					};

					ed::PinId startPinId = 0, endPinId = 0;

					//NOTE:ネストが深すぎませんか？
					if (ed::QueryNewLink(&startPinId, &endPinId))
						if (startPinId && endPinId)
							if (startPinId == endPinId)
							{
								ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
							}
							else
							{
								showLabel("+ Create Link", ImColor(32, 45, 32, 180));
								if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
								{
									TNodeManagerRef->MakeLink<ed::PinId>(startPinId, endPinId);
								}
							}
				}
				ed::EndCreate();
			}
		}

		
		ed::End();
		ed::SetCurrentEditor(nullptr);
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::LoadFile(fs::path _path)
	{
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::AddNodeSignature(TNodeSignature<SOCKET_TYPE> _nodeSig)
	{
		nodeSignatureVector.push_back(_nodeSig);
	}

	class TestEditor :public Editor {
	private:
		using TNodeEditor = NodeEditorBase<Episode_Event_Node, Socket_Data_Type>;
		TNodeEditor nodeEditor;

	public:
		TestEditor() :
			Editor("TestEditor"),
			nodeEditor(TNodeEditor("TestNodeEditor"))
		{};

		void Tick()	 override;
	};

	inline void teller::TestEditor::Tick()
	{
		ImGui::Begin("TestEditor");
		ImGui::BeginChild("testNodeed");

		nodeEditor.Tick();
		
		ImGui::EndChild();
		ImGui::End();
	}

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