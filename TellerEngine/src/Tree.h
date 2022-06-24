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

	enum class Socket_TYPE {
		Delegate,
		BOOL,
		INT,
		STRING,
		COMBO,
		OPTION,
		FLOW
	};

	//前方宣言
	template<class T>
	class TNodeCore;

	template<class T>
	struct TSocketCore {
		TSocketID ID_;

		std::weak_ptr<TNodeCore<T>> parentTNode;
		std::vector<TSocketID> targetSocketsID_;

		Socket_TYPE type_;
		TSocketCore() = delete;
		TSocketCore(Socket_TYPE _type,TSocketID _id) :
			type_(_type),
			ID_(_id)
		{};

		bool AddTarget(uint64_t _target);
	};

	template<class T>
	inline bool TSocketCore<T>::AddTarget(uint64_t _target)
	{
		targetSocketsID_.push_back(_target);
		return true;
		/*if (_target->parentTNode.lock().get() == this->parentTNode.lock().get()) {
			return false;
		}
		if (_target->kind_ == this->kind_) return false;
		targetSockets_.push_back(_target);
		return true;*/
	}

	//TNodeCoreここから


	template<class T>
	class TNodeCore :public std::enable_shared_from_this<TNodeCore<T>> {
	private:
		std::string description; //説明

	protected:
	public:
		std::string name_; //ノード名前
		ImVec2 size_; //大きさ
		ImVec2 pos_; //座標
		TNodeID ID_;
		TEventID eventID_;
		TEposodeID episodeID_;
		Node_TYPE type_;

		//ポインタじゃなくていいんじゃない？
		std::vector<std::shared_ptr<TSocketCore<T>>> socketsInput;
		std::vector<std::shared_ptr<TSocketCore<T>>> socketsOutput;


		TNodeCore() = delete;

		TNodeCore(Node_TYPE _type, std::string _title ,TNodeID _id, vec2 _pos=vec2(0,0), vec2 _size=vec2(0,0)) :
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

		void AddInputSocket(Socket_TYPE _scktType,TSocketID _id);
		void AddOutPutSocket(Socket_TYPE _scktType,TSocketID _id);
	};

	template<class T>
	inline void TNodeCore<T>::SetDesciption(std::string _description) { description = _description; }

	template<class T>
	inline void TNodeCore<T>::SetName(std::string _name) { name_ = _name; }

	template<class T>
	inline void TNodeCore<T>::AddInputSocket(Socket_TYPE _scktType, TSocketID _id)
	{
		auto sckt = std::make_shared<TSocketCore<T>>(_scktType, _id);
		sckt->parentTNode = this->shared_from_this();
		socketsInput.push_back(sckt);
	}

	template<class T>
	inline void TNodeCore<T>::AddOutPutSocket(Socket_TYPE _scktType, TSocketID _id)
	{
		auto sckt = std::make_unique<TSocketCore<T>>(_scktType, _id);
		sckt->parentTNode = this->shared_from_this();
		socketsOutput.push_back(std::move(sckt));
	}
	//TNodeCoreここまで

	struct TEventNodeSignature {
		TEventNodeSignature() = default;
		~TEventNodeSignature() = default;
	};

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

	//ノード管理クラス
	template<class T>
	class TNodeManager {
		uint64_t MakeUID_ui64t();

		utils::UIDGenerator uid;
		std::vector<TLinkInfo> tLinks;

		std::shared_ptr<TNodeCore<T>> beginNode_;
		std::shared_ptr<TNodeCore<T>> endNode_;

		void AddDataSlot();
	public:
		TNodeManager() :
			uid(utils::UIDGenerator())
		{};

		std::unordered_map<TNodeID, std::shared_ptr<TNodeCore<T>>> nodes;
		std::vector<TLinkInfo> GetLinkVector();

		template<typename U>
		void MakeLink(U _from, U _dest) {
			tLinks.emplace_back(uid.Generate(),(TSocketID)_from,(TSocketID)_dest);
		};

		std::shared_ptr<TSocketCore<T>> FindSocket(TSocketID _ID);

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
	};

	template<class T>
	inline void teller::TNodeManager<T>::AddDataSlot() {
	}

	template<class T>
	inline uint64_t teller::TNodeManager<T>::MakeUID_ui64t() 
	{
		return uid.Generate();
	}
	template<class T>
	inline std::vector<TLinkInfo> TNodeManager<T>::GetLinkVector()
	{
		auto links = std::vector<TLinkInfo>();
		int l = 1;
		for (auto iter = nodes.begin(); iter != nodes.end(); ++iter) {
			l += iter->second->socketsInput.size() + iter->second->socketsOutput.size();
		}

		auto mx = std::make_unique<Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic>>();

		return std::vector<TLinkInfo>();
	}

	template<class T>
	inline std::shared_ptr<TSocketCore<T>> TNodeManager<T>::FindSocket(TSocketID _ID)
	{
		for (auto& node : nodes)
		{
			auto insckt = std::find(node.second->socketsInput.begin(), node.second->socketsInput.end(), _ID);
			if (insckt != node.second->socketsInput.end())
				return *insckt;
			auto outsckt = std::find(node.second->socketsOutput.begin(), node.second->socketsOutput.end(), _ID);
			if (outsckt != node.second->socketsOutput.end())
				return *outsckt;
		}

		return nullptr;
	}

	template<class T>
	inline TNodeID TNodeManager<T>::AddTNodeBranch()
	{
		auto n = std::make_shared<TNodeCore<T>>(Node_TYPE::BRANCH, "Branch", MakeUID_ui64t());
		n->name_ = "Branch node";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		nodes[n->ID_] = n;

		return n->ID_;
	}

	template<class T>
	inline TNodeID TNodeManager<T>::AddTNodeSceneChange()
	{
		auto n = std::make_shared<TNodeCore<T>>(Node_TYPE::SCENECHANGE, "Scenechange ", MakeUID_ui64t());
		n->name_ = "Scene change.";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		nodes[n->ID_] = n;

		return n->ID_;
	}

	template<class T>
	inline TNodeID TNodeManager<T>::AddTNodeEvent()
	{
		auto n = std::make_shared<TNodeCore<T>>(Node_TYPE::EVENT, "Event", MakeUID_ui64t());
		n->name_ = "Event node.";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());

		nodes[n->ID_] = n;
		return n->ID_;

	}

	template<class T>
	inline TNodeID TNodeManager<T>::AddTNodeAnimation()
	{
		auto n = std::make_shared<TNodeCore<T>>(Node_TYPE::ANIMATION, "animation", MakeUID_ui64t());
		n->name_ = "Animation node.";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());

		nodes[n->ID_] = n;
		return n->ID_;
	}

	template<class T>
	inline TNodeID TNodeManager<T>::AddTNodeBegin()
	{
		auto n = std::make_shared<TNodeCore<T>>(Node_TYPE::BEGINEPISODE, "Begin", MakeUID_ui64t());
		n->name_ = "Episode begin.";
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		std::cout << n->ID_ << std::endl;

		beginNode_ = n;
		return n->ID_;
	}

	template<class T>
	inline TNodeID TNodeManager<T>::AddEpisodeNode(uint64_t _id)
	{
		auto n = std::make_shared<TNodeCore<T>>(Node_TYPE::EPISODE, "Episode", MakeUID_ui64t());

		n->name_ = "Episode";
		n->episodeID_ = _id;
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		auto nid = n->ID_;

		nodes[n->ID_] = n;
		return nid;
	}

	template<class T>
	inline TNodeID TNodeManager<T>::AddTNodeCharacterInOut()
	{
		auto n = std::make_shared<TNodeCore<T>>(Node_TYPE::CHARACTER_IN_OUT, "CharacterIn", MakeUID_ui64t());
		n->name_ = "Character In Out";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		auto nid = n->ID_;
		nodes[n->ID_] = n;

		return nid;
	}

	template<class T>
	inline TNodeID TNodeManager<T>::AddTNodeCharacterAppear(std::string _name)
	{

		return TNodeID();
	}

	template<class T>
	inline TNodeID TNodeManager<T>::AddTNodeCharacterExit(std::string _name)
	{
		return TNodeID();
	}

	template<class T>
	inline TNodeID TNodeManager<T>::AddEpisodeNode(uint64_t _id, int _in, int _out)
	{
		return TNodeID();
	}

	template<class T>
	inline TNodeID TNodeManager<T>::AddEpisodeNode(Episode _episode)
	{
		return TNodeID();
	}

	template<class T>
	inline TNodeID TNodeManager<T>::AddEpisodeNode(std::shared_ptr<Episode> _episode)
	{
		return -1;
	}


	template<class T>
	inline TNodeID TNodeManager<T>::AddTNodeEnd()
	{
		auto n = std::make_shared<TNodeCore<T>>(Node_TYPE::ENDEPISODE, "End", MakeUID_ui64t());
		n->name_ = "Episode endF";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		endNode_ = n;
		return n->ID_;
	}

	template<class T>
	inline TNodeID teller::TNodeManager<T>::AddTNodeCharacterSelecter()
	{
		auto n = std::make_unique<TNodeCore<T>>(Node_TYPE::BLANK, "Character Selecter", MakeUID_ui64t());
		n->name_ = "Character Selecter";
		n->AddOutPutSocket(Socket_TYPE::STRING, MakeUID_ui64t());
		auto nid = n->ID_;
		nodes[n->ID_] = std::move(n);

		return nid;
	}
}