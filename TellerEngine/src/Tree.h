#pragma once
#include<vector>
#include<memory>
#include<string>
#include<map>
#include<random>
#include<Eigen>
#include<algorithm>

#include"cinder/CinderImGui.h"
#include"imgui_node_editor.h"
#include"Episode.h"
#include"utility.h"


namespace teller {
	using namespace ci;
	using TSocketID = uint64_t;
	using TNodeID = uint64_t;
	using TLinkID = uint64_t;
	using TEventID = uint64_t;
	using TEposodeID = uint64_t;
	using TDataID = uint64_t;

	namespace ed = ax::NodeEditor;

	enum class Node_TYPE {
		BEGINEPISODE,
		ENDEPISODE,
		BLANK,
		EPISODE,
		EVENT,
		BRANCH,
		COMMENT,
		SCENECHANGE,
		ANIMATION,
		CHARACTER_IN_OUT
	};

	enum class Node_DATA
	{
		BOOL,
		INT,
		FLOAT,
		STRING,
		COMBO
	};

	enum class Socket_TYPE {
		Delegate,
		BOOL,
		INT,
		STRING,
		OPTION,
		FLOW
	};

	//テンプレート化する必要なかったな。

	class TNodeCore;

	struct TSocketCore {
		TSocketID ID_;

		std::weak_ptr<TNodeCore> parentTNode;
		std::vector<TSocketID> targetSocketsID_;

		Socket_TYPE type_;
		TSocketCore() = delete;
		TSocketCore(Socket_TYPE _type, TSocketID _id) :
			type_(_type),
			ID_(_id)
		{};

		bool operator==(const TSocketCore& rhs)
		{
			return ID_ == rhs.ID_ ? true : false;
		}

		bool operator!=(const TSocketCore& rhs)
		{
			return ID_ != rhs.ID_ ? true : false;
		}

		bool AddTarget(uint64_t _target);
	};


	inline bool TSocketCore::AddTarget(uint64_t _target)
	{
		targetSocketsID_.push_back(_target);
		return true;
	}

	class TNodeCore :public std::enable_shared_from_this<TNodeCore> {
	private:
		std::string description; //説明
		std::vector<EventDataCore> dataVec_;
		ImVec2 size_; //大きさ
		ImVec2 pos_; //座標

	protected:
	public:
		std::string name_; //ノード名前
		TNodeID ID_;
		TEventID eventID_;
		TEposodeID episodeID_;
		Node_TYPE type_;

		//ポインタじゃなくていいんじゃない？
		std::unordered_map<TSocketID, std::shared_ptr<TSocketCore>> socketsInput;
		std::unordered_map<TSocketID, std::shared_ptr<TSocketCore>> socketsOutput;

		TNodeCore() = delete;

		TNodeCore(Node_TYPE _type, std::string _title, TNodeID _id, vec2 _pos = vec2(0, 0), vec2 _size = vec2(0, 0)) :
			ID_(_id),
			type_(_type),
			name_(_title),
			description(""),
			pos_(_pos),
			size_(_size),
			eventID_(-1),
			episodeID_(-1)
		{};

		void SetName(std::string _name);
		void SetDesciption(std::string _description);

		void AddInputSocket(Socket_TYPE _scktType, TSocketID _id);
		void AddOutputSocket(Socket_TYPE _scktType, TSocketID _id);

		void SetPosition(vec2 _pos) { pos_ = _pos; }
	};

	inline void TNodeCore::SetDesciption(std::string _description) { description = _description; }


	inline void TNodeCore::SetName(std::string _name) { name_ = _name; }

	inline void TNodeCore::AddInputSocket(Socket_TYPE _scktType, TSocketID _id)
	{
		auto sckt = std::make_unique<TSocketCore>(_scktType, _id);
		sckt->parentTNode = this->shared_from_this();
		socketsInput[sckt->ID_] = std::move(sckt);
	}

	inline void TNodeCore::AddOutputSocket(Socket_TYPE _scktType, TSocketID _id)
	{
		auto sckt = std::make_unique<TSocketCore>(_scktType, _id);
		sckt->parentTNode = this->shared_from_this();

		socketsOutput[sckt->ID_] = std::move(sckt);
	}

	struct TLinkInfo
	{
		TLinkID ID_;
		TSocketID  InputID_;
		TSocketID  OutputID_;
		TLinkInfo(TLinkID _ID, TSocketID _InID, TSocketID _OutID) :
			ID_(_ID),
			InputID_(_InID),
			OutputID_(_OutID) {};
		~TLinkInfo() = default;
	};

	//これをテンプレートととして
	struct TNodeSignature {
		std::string name;
		std::string description;
		std::vector<Socket_TYPE> inputSockets;
		std::vector<Socket_TYPE> outputSockets;
		TNodeSignature():
			name(""),
			description("")
		{};
		TNodeSignature(std::string _name):
			name(_name),
			description("")
		{};
		virtual ~TNodeSignature() = default;
	};

	//ノード管理クラス

	class TNodeManager {
		uint64_t MakeUID_ui64t();

		utils::UIDGenerator uid;
		std::vector<TLinkInfo> tLinks;

		std::shared_ptr<TNodeCore> beginNode_;
		std::shared_ptr<TNodeCore> endNode_;

		std::unordered_map<std::string, TNodeSignature> nodeSignatureVector;

	public:
		TNodeManager() :
			uid(utils::UIDGenerator())
		{};

		TNodeManager(fs::path _path) :
			uid(utils::UIDGenerator())
		{};

		std::unordered_map<TNodeID, std::shared_ptr<TNodeCore>> nodes;

		std::vector<TLinkInfo> GetLinkVector();

		template<typename U>
		void MakeLink(U _from, U _dest) {
			tLinks.emplace_back(uid.Generate(), (TSocketID)_from, (TSocketID)_dest);
		};

		std::shared_ptr<TSocketCore> FindSocket(TSocketID _ID);

		//It returns a copy of std::vector<TLinkInfo>.
		//Use MakeLink if you want to create a new link.
		std::vector<TLinkInfo> GetLinks()const { return tLinks; };

		//ノードにデータスロットを追加
		void AddDataSlot(TDataType _type);

		//分岐やイベントなどの基本ノード
		TNodeID AddTNodeBranch();
		TNodeID AddTNodeSceneChange();
		TNodeID AddTNodeEvent();
		TNodeID AddTNodeAnimation();
		TNodeID AddTNodeEnd();
		TNodeID AddTNodeBegin();
		TNodeID AddEpisodeNode(uint64_t _id);
		TNodeID AddTNodeCharacterInOut();
		TNodeID AddTNodeCharacterAppear(std::string _name);
		TNodeID AddTNodeCharacterExit(std::string _name);
		TNodeID AddEpisodeNode(uint64_t _id, int _in, int _out);
		TNodeID AddEpisodeNode(Episode _episode);
		TNodeID AddEpisodeNode(std::shared_ptr<Episode> _episode);

		//基本ノードを補助するノード
		TNodeID AddTNodeCharacterSelecter();

		void SetNodePosition(TNodeID _ID, vec2 _pos)
		{
		}

		void Tick();

		//Depricated:ノードエディターのAddNodeSignatureを使え
		//Use a method AddNodeSignature in NodeEditorBase
		void AddNodeSignature(TNodeSignature _nodeSignature);
	};

	inline void teller::TNodeManager::AddNodeSignature(TNodeSignature _nodeSignature)
	{
		nodeSignatureVector[_nodeSignature.name] = _nodeSignature;
	}

	inline uint64_t teller::TNodeManager::MakeUID_ui64t()
	{
		return uid.Generate();
	}

	inline std::vector<TLinkInfo> TNodeManager::GetLinkVector()
	{
		auto links = std::vector<TLinkInfo>();
		int l = 1;
		for (auto iter = nodes.begin(); iter != nodes.end(); ++iter) {
			l += iter->second->socketsInput.size() + iter->second->socketsOutput.size();
		}

		auto mx = std::make_unique<Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic>>();

		return std::vector<TLinkInfo>();
	}

	inline std::shared_ptr<TSocketCore> TNodeManager::FindSocket(TSocketID _ID)
	{
		for (auto& node : nodes)
		{
			auto insckt = node.second->socketsInput.find(_ID);
			if (insckt != node.second->socketsInput.end())
				return insckt->second;
			auto outsckt = node.second->socketsOutput.find(_ID);
			if (outsckt != node.second->socketsOutput.end())
				return outsckt->second;
		}

		return nullptr;
	}


	inline TNodeID TNodeManager::AddTNodeBranch()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::BRANCH, "Branch", MakeUID_ui64t());
		n->name_ = "Branch node";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		nodes[n->ID_] = n;

		return n->ID_;
	}

	inline TNodeID TNodeManager::AddTNodeSceneChange()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::SCENECHANGE, "Scenechange ", MakeUID_ui64t());
		n->name_ = "Scene change.";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		nodes[n->ID_] = n;

		return n->ID_;
	}

	inline TNodeID TNodeManager::AddTNodeEvent()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::EVENT, "Event", MakeUID_ui64t());
		n->name_ = "Event node.";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());

		nodes[n->ID_] = n;
		return n->ID_;

	}

	inline TNodeID TNodeManager::AddTNodeAnimation()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::ANIMATION, "animation", MakeUID_ui64t());
		n->name_ = "Animation node.";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());

		nodes[n->ID_] = n;
		return n->ID_;
	}


	inline TNodeID TNodeManager::AddTNodeBegin()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::BEGINEPISODE, "Begin", MakeUID_ui64t());
		n->name_ = "Episode begin.";
		n->AddOutputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		std::cout << n->ID_ << std::endl;

		beginNode_ = n;
		return n->ID_;
	}

	inline TNodeID TNodeManager::AddEpisodeNode(uint64_t _id)
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::EPISODE, "Episode", MakeUID_ui64t());

		n->name_ = "Episode";
		n->episodeID_ = _id;
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		auto nid = n->ID_;

		nodes[n->ID_] = n;
		return nid;
	}

	inline TNodeID TNodeManager::AddTNodeCharacterInOut()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::CHARACTER_IN_OUT, "CharacterIn", MakeUID_ui64t());
		n->name_ = "Character In Out";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		auto nid = n->ID_;
		nodes[n->ID_] = n;

		return nid;
	}

	inline TNodeID TNodeManager::AddTNodeCharacterAppear(std::string _name)
	{

		return TNodeID();
	}

	inline TNodeID TNodeManager::AddTNodeCharacterExit(std::string _name)
	{
		return TNodeID();
	}

	inline TNodeID TNodeManager::AddEpisodeNode(uint64_t _id, int _in, int _out)
	{
		return TNodeID();
	}

	inline TNodeID TNodeManager::AddEpisodeNode(Episode _episode)
	{
		return TNodeID();
	}

	inline TNodeID TNodeManager::AddEpisodeNode(std::shared_ptr<Episode> _episode)
	{
		return -1;
	}

	inline TNodeID TNodeManager::AddTNodeEnd()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::ENDEPISODE, "End", MakeUID_ui64t());
		n->name_ = "Episode endF";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		endNode_ = n;
		return n->ID_;
	}

	inline TNodeID teller::TNodeManager::AddTNodeCharacterSelecter()
	{
		auto n = std::make_unique<TNodeCore>(Node_TYPE::BLANK, "Character Selecter", MakeUID_ui64t());
		n->name_ = "Character Selecter";
		n->AddOutputSocket(Socket_TYPE::STRING, MakeUID_ui64t());
		auto nid = n->ID_;
		nodes[n->ID_] = std::move(n);

		return nid;
	}

}