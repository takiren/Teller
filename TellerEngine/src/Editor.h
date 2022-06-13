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

	//�G�C���A�X
	namespace ed = ax::NodeEditor;
	namespace fs = std::filesystem;
	using namespace ax;
	using ax::Widgets::IconType;

	//�G�f�B�^�[���Ǘ�����N���X������΂����̂��Ȃ��Ďv�������ǂ���Ȃ����炢����폜
	class EditorManager :public std::enable_shared_from_this<EditorManager> {
		//TellerCore�ւ�weak ptr
		std::weak_ptr<TellerCore> parent;
	public:
		EditorManager() {};
		virtual ~EditorManager() = default;
		void EditorCallBack(std::string _filename);
		void EditorCallBack(std::unique_ptr<Episode> _episode);
	};

	//��������Ȃ�
	enum class EDITOR_TYPE
	{
		NODE_EDITOR,
		EPISODE_EDITOR
	};

	class Editor :public std::enable_shared_from_this<Editor> {
	protected:
		//true�̂Ƃ�tick���Ăяo���B
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

		//�R�s�[�֎~
		Editor(const Editor&) = delete;
		Editor& operator=(const Editor&) = delete;
		//���[�u����
		Editor& operator=(Editor&&) = default;

		//depricated
		virtual void TickInternal();

		//���`�b�N�Ă΂�鏈��
		virtual void Tick();

		//�����炭�K�v�Ȃ�
		virtual void Update();

		//�G�f�B�^�[�ł̕ҏW��ۑ�����
		virtual void Save();

		//����Ȃ�
		virtual bool CanAccept(fs::path _path) = 0;

		//���b�Z�[�W�p
		virtual void GetTMessage(TEVENT_MESSAGE& _message) {};

		virtual void CallByParent();

		//���[�h
		virtual void LoadFile(fs::path _path) = 0;

		//�Q�� : imgui-node-editor->Splitter
		virtual bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f);
	};

	//���j���[�o�[�p

	class TopLevelMenu :public Editor {
	private:
	public:
		TopLevelMenu() : Editor("TopLevelMenu") {};
		void Tick() override;
		void LoadFile(fs::path _path) override;
	};

	//csv����G�s�\�[�h�t�@�C�����쐬���邽�߂̃G�f�B�^�[
	class EpisodeEditor :public Editor {
	private:
		std::pair<int, int> lineBracket;

		//�t�@�C�����X�g
		std::string episodeNameCandidate;

		std::unique_ptr<CSVLoader> CSVRef;

		//�������p�֐��B
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

	//�G�s�\�[�h�C�x���g�G�f�B�^�[
	class EpisodeEventEditor :public Editor {
	private:
		//�m�[�h�G�f�B�^�p�ϐ�
		ImColor GetIconColor(Socket_TYPE type);
		void DrawPinIcon(const std::shared_ptr<TSocketCore> sckt, bool connected, int alpha);
		int s_PinIconSize;

		//���̃p�l����\��
		void ShowLeftPane(float panewidth);
		bool g_FirstFrame = true;

		void OpenAddNodePopup();

		//�m�[�h�G�f�B�^�p�ϐ�
		ImVector<LinkInfo> g_Links;
		ed::EditorContext* gContext;

		uint64_t currentEpisodeID_;

		std::unique_ptr<TNodeManager> TNodeManagerRef;

		std::vector<std::string> nodeList_;
		std::unique_ptr<Episode> episodeRef;

		//�ҏW�Ώۂ̍s
		int currentLine;

		//�L�����N�^�[������
		int characterAppearanceNum;
		//EventPack�ҏW�s
		int eventPackNum;

		//json
		json jsonEpisode;
		std::unordered_map<std::string, json> jsonCharacterMap;
		std::unordered_map<std::string, fs::path> characterPathMap;

		using EventPack = std::vector<std::unique_ptr<EpisodeEvent>>;
		std::map<int, EventPack> eventPackMap;

		fs::path jsonFilePath_;

		void CreateEpisodeEvent(EPISODE_EVENT_TYPE _type, int _line, std::string _target, std::string _key);
		void SwapEvent(EventPack& _vector, int m, int n);
		void Initialize();
		void LoadEpisode(fs::path _path);
		void LoadCharacterJson(fs::path _path);
		void LoadEpisodeEvent(json _j);

		//Character.json��file�����擾
		std::vector<std::string> GetSpritesName(json _cjson);

		bool bShowSprite;

	public:
		EpisodeEventEditor() :
			Editor("EpisodeEventEditor"),
			TNodeManagerRef(std::move(std::make_unique<TNodeManager>())),
			gContext(ed::CreateEditor()),
			currentEpisodeID_(0),
			episodeRef(nullptr),
			jsonEpisode(nullptr),
			currentLine(0),
			s_PinIconSize(24),
			characterAppearanceNum(0),
			eventPackNum(0),
			bShowSprite(true)
		{
			//�m�[�h�p
			nodeList_.push_back("Branch.");
			nodeList_.push_back("Scene change");
			nodeList_.push_back("Animation.");
			nodeList_.push_back("Episode.");
			nodeList_.push_back("Event.");
			nodeList_.push_back("Comment");
			nodeList_.push_back("Character In Out");
		}
		~EpisodeEventEditor() = default;

		void Tick() override;
		void Update() override;

		void CallByParent() override;

		//TODO: Delete
		void UpdateAssetList();
		//TODO: Delete
		bool CanAccept(fs::path _path) override;

		void LoadFile(fs::path _path) override;

		//TODO:�����ǂ��ɂ�����
		void Save()override;
	};

	//�m�[�h�G�f�B�^�p���N���X
	//TODO:�����𑁂��������邩�폜����
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
		// EpisodeManager�ւ̃|�C���^�B
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

		//�A�Z�b�g�ւ̃p�X
		fs::path episodePath_;
		fs::path characterPath_;
		void Initialize();

		fs::path targetFile;

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