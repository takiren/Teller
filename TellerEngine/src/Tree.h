#pragma once
#include<vector>
#include<memory>
#include<string>
#include<map>
#include<random>
#include<Eigen>
#include<algorithm>

#include"cinder/CinderImGui.h"
#include<utilities/widgets.h>
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
	using ax::Widgets::IconType;

	enum class Episode_Event_Node {
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
		COMBO,
		AGENT,
		ANIMATION
	};

	enum class Socket_Data_Type {
		Delegate,
		BOOL,
		INT,
		VEC2,
		FLOAT,
		STRING,
		OPTION,
		FLOW,
		ANIMATION,
		AGENT
	};

	enum class Episode_Event_Socket {
		BOOL,
		INT,
		VEC2,
		FLOAT,
		STRING,
		OPTION,
		FLOW,
		ANIMATION,
		AGENT
	};

	//テンプレート化する必要なかったな。

	//Event用のデータ
	using TEData = std::string;

	//共用体を使えばシンプルになる
	class EventDataCore {
	protected:
		Node_DATA type_;
		TEData data_;
	public:
		EventDataCore() = delete;
		EventDataCore(Node_DATA _type) :
			type_(_type),
			data_("")
		{};

		EventDataCore(Node_DATA _type, TEData _data) :
			type_(_type),
			data_(_data)
		{};
		virtual ~EventDataCore() = default;

		TEData GetDump()const { return data_; };

		template<typename T>
		inline T GetData()
		{
			return data_;
		}

		template<typename T>
		inline void SetData(T _data)
		{
			return T();
		}
	};

	template<>
	inline void EventDataCore::SetData<std::string>(std::string _data)
	{
		data_ = _data;
	}

	template<>
	inline void EventDataCore::SetData<ci::vec2>(ci::vec2 _data)
	{
		std::stringstream ss;
		ss << _data[0] << "," << _data[1];
		data_ = ss.str();
	}

	template<>
	inline void EventDataCore::SetData<int>(int _data)
	{
		data_ = _data;
	}

	template<>
	inline void EventDataCore::SetData<float>(float _data)
	{
		data_ = _data;
	}

	template<>
	inline std::string EventDataCore::GetData<std::string>()
	{
		return data_;
	}

	template<>
	inline ci::vec2 EventDataCore::GetData<ci::vec2>()
	{
		auto strvec = Utility::split(data_, ',');
		return ci::vec2(std::stof(strvec.at(0)), std::stof(strvec.at(1)));
	}

	template<>
	inline int EventDataCore::GetData<int>()
	{
		return std::stoi(data_);
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	class TNodeCore;


	template<class NODE_TYPE, class SOCKET_TYPE>
	struct TSocketCore {
		TSocketID ID_;

		std::weak_ptr<TNodeCore<NODE_TYPE, SOCKET_TYPE>> parentTNode;
		std::vector<TSocketID> targetSocketsID_;

		IconType icontype_;

		SOCKET_TYPE type_;

		TSocketCore() = delete;
		TSocketCore(SOCKET_TYPE _type, TSocketID _id, IconType icontype = IconType::Flow) :
			type_(_type),
			ID_(_id),
			icontype_(icontype)
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

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline bool TSocketCore<NODE_TYPE, SOCKET_TYPE>::AddTarget(uint64_t _target)
	{
		targetSocketsID_.push_back(_target);
		return true;
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	class TNodeCore :public std::enable_shared_from_this<TNodeCore<NODE_TYPE, SOCKET_TYPE>> {
	private:
		std::string description; //説明
		std::vector<EventDataCore> dataVec_;
		ImVec2 size_; //大きさ
		ImVec2 pos_; //座標

	protected:
	public:
		using TSocket = TSocketCore<NODE_TYPE, SOCKET_TYPE>;

		std::string name_; //ノード名前
		TNodeID ID_;
		TEventID eventID_;
		TEposodeID episodeID_;
		NODE_TYPE type_;

		//ポインタじゃなくていいんじゃない？
		std::unordered_map<TSocketID, std::shared_ptr<TSocket>> socketsInput;
		std::unordered_map<TSocketID, std::shared_ptr<TSocket>> socketsOutput;

		TNodeCore() = delete;

		TNodeCore(NODE_TYPE _type, std::string _title, TNodeID _id, vec2 _pos = vec2(0, 0), vec2 _size = vec2(0, 0)) :
			ID_(_id),
			type_(_type),
			name_(_title),
			description(""),
			pos_(_pos),
			size_(_size),
			eventID_(-1),
			episodeID_(-1)
		{
		};

		void SetName(std::string _name);
		void SetDesciption(std::string _description);

		void AddInputSocket(SOCKET_TYPE _scktType, TSocketID _id);
		void AddInputSocket(SOCKET_TYPE _scktType, TSocketID _id, IconType icontype);

		void AddOutputSocket(SOCKET_TYPE _scktType, TSocketID _id);
		void AddOutputSocket(SOCKET_TYPE _scktType, TSocketID _id, IconType icontype);


		void SetPosition(vec2 _pos) { pos_ = _pos; }

	};

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void TNodeCore< NODE_TYPE, SOCKET_TYPE>::SetDesciption(std::string _description) { description = _description; }


	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void TNodeCore< NODE_TYPE, SOCKET_TYPE>::SetName(std::string _name) { name_ = _name; }

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void TNodeCore< NODE_TYPE, SOCKET_TYPE>::AddInputSocket(SOCKET_TYPE _scktType, TSocketID _id)
	{
		auto sckt = std::make_unique<TSocket>(_scktType, _id);
		sckt->parentTNode = this->shared_from_this();
		socketsInput[sckt->ID_] = std::move(sckt);
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void TNodeCore<NODE_TYPE, SOCKET_TYPE>::AddInputSocket(SOCKET_TYPE _scktType, TSocketID _id, IconType icontype)
	{
		auto sckt = std::make_unique<TSocket>(_scktType, _id, icontype);
		sckt->parentTNode = this->shared_from_this();

		socketsOutput[sckt->ID_] = std::move(sckt);
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void TNodeCore< NODE_TYPE, SOCKET_TYPE>::AddOutputSocket(SOCKET_TYPE _scktType, TSocketID _id)
	{
		auto sckt = std::make_unique<TSocket>(_scktType, _id);
		sckt->parentTNode = this->shared_from_this();

		socketsOutput[sckt->ID_] = std::move(sckt);
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void TNodeCore<NODE_TYPE, SOCKET_TYPE>::AddOutputSocket(SOCKET_TYPE _scktType, TSocketID _id, IconType icontype)
	{
		auto sckt = std::make_unique<TSocket>(_scktType, _id, icontype);
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
	template<class SOCKET_TYPE>
	struct TNodeSignature {
		std::string name;
		std::string description;

		std::vector< std::pair< SOCKET_TYPE, IconType> > inputSockets;
		std::vector< std::pair< SOCKET_TYPE, IconType> > outputSockets;

		TNodeSignature() = delete;

		TNodeSignature(std::string _name) :
			name(_name),
			description("")
		{};

		virtual ~TNodeSignature() = default;

		//自身が辞書的に上位に位置するのであればtrue
		bool operator >(TNodeSignature& rhs) const {
			return name.compare(rhs.name) < 0;
		}

		//自身が辞書的に下位に位置するのであればtrue
		bool operator <(TNodeSignature& rhs) const {
			return name.compare(rhs.name) > 0;
		}

	};


	//ノード管理クラス

	template<class NODE_TYPE, class SOCKET_TYPE>
	class TNodeManager {
		using TSocket = TSocketCore<NODE_TYPE, SOCKET_TYPE>;
		using TNode = TNodeCore<NODE_TYPE, SOCKET_TYPE>;

		uint64_t MakeUID_ui64t();

		utils::UIDGenerator uid;
		std::vector<TLinkInfo> tLinks;


		std::shared_ptr<TNode> beginNode_;
		std::shared_ptr<TNode> endNode_;

		std::unordered_map<std::string, TNodeSignature<SOCKET_TYPE>> nodeSignatureVector;

		template<class NODE_TYPE,class SOCKET_TYPE>
		struct TNodeSequence {
			struct Connection
			{

			};
		};

	public:
		TNodeManager() :
			uid(utils::UIDGenerator())
		{
		};

		std::unordered_map<TNodeID, std::shared_ptr<TNode>> nodes;

		std::vector<TLinkInfo> GetLinkVector();

		template<typename U>
		void MakeLink(U _from, U _dest) {
			tLinks.emplace_back(uid.Generate(), (TSocketID)_from, (TSocketID)_dest);
		};

		std::shared_ptr<TSocket> FindSocket(TSocketID _ID);

		//It returns a copy of std::vector<TLinkInfo>.
		//Use MakeLink if you want to create a new link.
		std::vector<TLinkInfo> GetLinks()const { return tLinks; };

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

		//Depricated:ノードエディターのAddNodeSignatureを使え
		//Use a method AddNodeSignature in NodeEditorBase
		void AddNodeSignature(TNodeSignature<SOCKET_TYPE> _nodeSignature);

		TNodeID AddNodeFromSignature(TNodeSignature<SOCKET_TYPE> _nodeSignature);

		TNode testfunc();
	};

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeCore<NODE_TYPE, SOCKET_TYPE> TNodeManager<NODE_TYPE, SOCKET_TYPE>::testfunc()
	{

	};

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddNodeFromSignature(TNodeSignature<SOCKET_TYPE> _nodeSignature)
	{
		auto newnode = std::make_shared<TNode>(Episode_Event_Node::BLANK, _nodeSignature.name, uid.Generate());
		for (auto& sckt : _nodeSignature.inputSockets)
			newnode->AddInputSocket(sckt.first, uid.Generate());

		for (auto& sckt : _nodeSignature.outputSockets)
			newnode->AddOutputSocket(sckt.first, uid.Generate());

		auto nid = newnode->ID_;
		nodes[nid] = std::move(newnode);

		return nid;
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddNodeSignature(TNodeSignature<SOCKET_TYPE> _nodeSignature)
	{
		nodeSignatureVector[_nodeSignature.name] = _nodeSignature;
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::MakeUID_ui64t()
	{
		return uid.Generate();
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline std::vector<TLinkInfo> teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::GetLinkVector()
	{
		auto links = std::vector<TLinkInfo>();
		int l = 1;
		for (auto iter = nodes.begin(); iter != nodes.end(); ++iter) {
			l += iter->second->socketsInput.size() + iter->second->socketsOutput.size();
		}

		auto mx = std::make_unique<Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic>>();

		return std::vector<TLinkInfo>();
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline std::shared_ptr<TSocketCore<NODE_TYPE, SOCKET_TYPE>> teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::FindSocket(TSocketID _ID)
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

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddTNodeBranch()
	{
		auto n = std::make_shared<TNode>(Episode_Event_Node::BRANCH, "Branch", MakeUID_ui64t());
		n->name_ = "Branch node";
		n->AddInputSocket(Socket_Data_Type::FLOW, MakeUID_ui64t());
		n->AddOutputSocket(Socket_Data_Type::FLOW, MakeUID_ui64t());
		n->AddOutputSocket(Socket_Data_Type::FLOW, MakeUID_ui64t());
		nodes[n->ID_] = n;

		return n->ID_;
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddTNodeSceneChange()
	{
		auto n = std::make_shared<TNode>(Episode_Event_Node::SCENECHANGE, "Scenechange ", MakeUID_ui64t());
		n->name_ = "Scene change.";
		n->AddInputSocket(Socket_Data_Type::FLOW, MakeUID_ui64t());
		n->AddOutputSocket(Socket_Data_Type::FLOW, MakeUID_ui64t());
		nodes[n->ID_] = n;

		return n->ID_;
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddTNodeEvent()
	{
		auto n = std::make_shared<TNode>(Episode_Event_Node::EVENT, "Event", MakeUID_ui64t());
		n->name_ = "Event node.";
		n->AddInputSocket(Socket_Data_Type::FLOW, MakeUID_ui64t());
		n->AddOutputSocket(Socket_Data_Type::FLOW, MakeUID_ui64t());

		nodes[n->ID_] = n;
		return n->ID_;

	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddTNodeAnimation()
	{
		auto n = std::make_shared<TNode>(Episode_Event_Node::ANIMATION, "animation", MakeUID_ui64t());
		n->name_ = "Animation node.";
		n->AddInputSocket(Socket_Data_Type::FLOW, MakeUID_ui64t());
		n->AddOutputSocket(Socket_Data_Type::FLOW, MakeUID_ui64t());

		nodes[n->ID_] = n;
		return n->ID_;
	}


	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddTNodeBegin()
	{
		auto n = std::make_shared<TNode>(Episode_Event_Node::BEGINEPISODE, "Begin", MakeUID_ui64t());
		n->name_ = "Episode begin.";
		n->AddOutputSocket(Socket_Data_Type::FLOW, MakeUID_ui64t());
		std::cout << n->ID_ << std::endl;

		beginNode_ = n;
		return n->ID_;
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddEpisodeNode(uint64_t _id)
	{
		auto n = std::make_shared<TNode>(Episode_Event_Node::EPISODE, "Episode", MakeUID_ui64t());

		n->name_ = "Episode";
		n->episodeID_ = _id;
		n->AddInputSocket(Socket_Data_Type::FLOW, MakeUID_ui64t());
		n->AddOutputSocket(Socket_Data_Type::FLOW, MakeUID_ui64t());
		auto nid = n->ID_;

		nodes[n->ID_] = n;
		return nid;
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddTNodeCharacterInOut()
	{
		auto n = std::make_shared<TNode>(Episode_Event_Node::CHARACTER_IN_OUT, "CharacterIn", MakeUID_ui64t());
		n->name_ = "Character In Out";
		n->AddInputSocket(Socket_Data_Type::FLOW, MakeUID_ui64t());
		n->AddOutputSocket(Socket_Data_Type::FLOW, MakeUID_ui64t());
		auto nid = n->ID_;
		nodes[n->ID_] = n;

		return nid;
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddTNodeCharacterAppear(std::string _name)
	{

		return TNodeID();
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddTNodeCharacterExit(std::string _name)
	{
		return TNodeID();
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddEpisodeNode(uint64_t _id, int _in, int _out)
	{
		return TNodeID();
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddEpisodeNode(Episode _episode)
	{
		return TNodeID();
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddEpisodeNode(std::shared_ptr<Episode> _episode)
	{
		return -1;
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddTNodeEnd()
	{
		auto n = std::make_shared<TNode>(Episode_Event_Node::ENDEPISODE, "End", MakeUID_ui64t());
		n->name_ = "Episode endF";
		n->AddInputSocket(Socket_Data_Type::FLOW, MakeUID_ui64t());
		endNode_ = n;
		return n->ID_;
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::TNodeManager<NODE_TYPE, SOCKET_TYPE>::AddTNodeCharacterSelecter()
	{
		auto n = std::make_unique<TNode>(Episode_Event_Node::BLANK, "Character Selecter", MakeUID_ui64t());
		n->name_ = "Character Selecter";
		n->AddOutputSocket(Socket_Data_Type::STRING, MakeUID_ui64t());
		auto nid = n->ID_;
		nodes[n->ID_] = std::move(n);

		return nid;
	}

	struct EventSignature {
		std::vector<EventDataCore> data_;
		std::string name_;

		EventSignature() = default;
		virtual ~EventSignature() = default;

		void AddEventData(EventDataCore&& _data) { data_.push_back(_data); };
		void SetName(std::string _name) { name_ = _name; };
	};

	class EpisodeEvent {
		std::vector<EventDataCore> data_;

	public:
		EPISODE_EVENT_TYPE type_;
		uint64_t ID_;
		int targetLine;
		std::string target_;
		std::string key_;
		std::string description_;

		EpisodeEvent() = delete;
		EpisodeEvent(EPISODE_EVENT_TYPE _type, int _line, std::string _key) :
			type_(_type),
			targetLine(_line),
			key_(_key),
			ID_(-1),
			target_("")
		{};

		EpisodeEvent(EPISODE_EVENT_TYPE _type, int _line, std::string _target, std::string _key) :
			type_(_type),
			targetLine(_line),
			target_(""),
			key_(_key),
			ID_(-1)
		{};
	};

}