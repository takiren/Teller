#pragma once
#include<vector>
#include<memory>
#include<string>
#include<map>
#include<random>
#include<Eigen>

#include"cinder/CinderImGui.h"
#include"imgui_node_editor.h"
#include"Episode.h"
#include"utility.h"


namespace teller {
	using namespace ci;
	using TSocketID = uint64_t;
	using TNodeID = uint64_t;
	using TLinkID = uint64_t;

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
		OPTION,
		FLOW
	};

	//前方宣言
	class TNodeCore;

	struct TSocketCore {
		TSocketID ID_;

		std::weak_ptr<TNodeCore> parentTNode;
		std::vector<TSocketID> targetSocketsID_;
		Socket_TYPE type_;
		TSocketCore() = delete;
		TSocketCore(Socket_TYPE _type,TSocketID _id) :
			type_(_type),
			ID_(_id)
		{
		};

		bool AddTarget(uint64_t _target);
	};

	class TNodeCore :public std::enable_shared_from_this<TNodeCore> {
	protected:
	public:
		std::string title_; //ノード名前
		std::string description; //説明
		ImVec2 size_; //大きさ
		ImVec2 pos_; //座標
		TNodeID ID_;
		uint64_t eventID_;
		uint64_t episodeID_;
		Node_TYPE type_;

		//ポインタじゃなくていいんじゃない？
		std::vector<std::shared_ptr<TSocketCore>> socketsInput;
		std::vector<std::shared_ptr<TSocketCore>> socketsOutput;

		TNodeCore() = delete;

		TNodeCore(Node_TYPE _type, std::string _title ,TNodeID _id, vec2 _pos=vec2(0,0), vec2 _size=vec2(0,0)) :
			ID_(_id),
			type_(_type),
			title_(_title),
			description(""),
			pos_(_pos),
			size_(_size)
		{};

		void SetDesciption(std::string _description);
		void AddInputSocket(Socket_TYPE _scktType,TSocketID _id);

		void AddOutPutSocket(Socket_TYPE _scktType,TSocketID _id);
	};

	class TNodeAnimation :public TNodeCore {
	public:

	};

	class TNodeEvent :TNodeCore {

	};

	class TNodeSceneChange :public TNodeCore {
	public:

	};

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
	class TNodeManager {

		//TODO:teller::utils::UIDGeneratorで書き換え
		std::mt19937_64 engine;
		std::uniform_int_distribution<uint64_t> ui64distr; //shortened uint64_t distribution

		uint64_t MakeUID_ui64t();

		utils::UIDGenerator uid;
		std::vector<TLinkInfo> tLinks;
	public:
		TNodeManager() :
			engine(std::random_device{}()),
			uid(utils::UIDGenerator())
		{
		};

		std::shared_ptr<TNodeCore> beginNode_;
		std::shared_ptr<TNodeCore> endNode_;
		std::map<uint64_t, std::shared_ptr<TNodeCore>> nodes;

		std::vector<TLinkInfo> GetLinkVector();

		template<typename T>
		void MakeLink(T _from, T _dest) {
			tLinks.emplace_back(uid.Generate(),(TSocketID)_from,(TSocketID)_dest);
		};
		std::shared_ptr<TSocketCore> SearchSocket(TSocketID _ID);

		//It returns a copy of std::vector<TLinkInfo>.
		//Use MakeLink if you want to create a new link.
		std::vector<TLinkInfo> GetLinks()const { return tLinks; };

		TNodeID AddTNodeBranch();
		TNodeID AddTNodeSceneChange();
		TNodeID AddTNodeEvent();
		TNodeID AddTNodeAnimation();
		TNodeID AddTNodeEnd();
		TNodeID AddTNodeBegin();
		TNodeID AddEpisodeNode(uint64_t _id);
		TNodeID AddTNodeCharacterInOut();
		TNodeID AddEpisodeNode(uint64_t _id, int _in, int _out);
		TNodeID AddEpisodeNode(Episode _episode);
		TNodeID AddEpisodeNode(std::shared_ptr<Episode> _episode);
	};

	inline uint64_t teller::TNodeManager::MakeUID_ui64t() 
	{
		return ui64distr(engine);
	}
}