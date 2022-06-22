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



namespace teller {
	using namespace ci;
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
		uint64_t ID_;

		std::weak_ptr<TNodeCore> parentTNode;
		std::vector<uint64_t> targetSocketsID_;
		Socket_TYPE type_;
		TSocketCore() = delete;
		TSocketCore(Socket_TYPE _type,uint64_t _id) :
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
		uint64_t ID_;
		uint64_t eventID_;
		uint64_t episodeID_;
		Node_TYPE type_;

		//ポインタじゃなくていいんじゃない？
		std::vector<std::shared_ptr<TSocketCore>> socketsInput;
		std::vector<std::shared_ptr<TSocketCore>> socketsOutput;

		TNodeCore() = delete;

		TNodeCore(Node_TYPE _type, std::string _title ,uint64_t _id, vec2 _pos=vec2(0,0), vec2 _size=vec2(0,0)) :
			ID_(_id),
			type_(_type),
			title_(_title),
			description(""),
			pos_(_pos),
			size_(_size)
		{};

		void SetDesciption(std::string _description);
		void AddInputSocket(Socket_TYPE _scktType,uint64_t _id);

		void AddOutPutSocket(Socket_TYPE _scktType,uint64_t _id);
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

	struct LinkInfo
	{
		ed::LinkId Id;
		ed::PinId  InputId;
		ed::PinId  OutputId;
	};

	//ノード管理クラス
	class TNodeManager {
		std::weak_ptr<Episode> episode_;

		std::mt19937_64 engine;
		std::uniform_int_distribution<uint64_t> ui64distr;

		uint64_t MakeUID_ui64t();

	public:
		TNodeManager() :
			engine(std::random_device{}())
		{
		};

		void AddTNode(Node_TYPE _nodeType);
		std::shared_ptr<TNodeCore> beginNode_;
		std::shared_ptr<TNodeCore> endNode_;
		std::map<uint64_t, std::shared_ptr<TNodeCore>> nodes;

		std::vector<LinkInfo> GetLinkVector();
		void MakeLink(std::shared_ptr<TSocketCore> _from, uint64_t _dest);
		void MakeLink(uint64_t _from, uint64_t _dest);
		std::shared_ptr<TSocketCore> SearchSocket(uint64_t _ID);

		std::vector<LinkInfo> GetLinks();

		uint64_t AddTNodeBranch();
		uint64_t AddTNodeSceneChange();
		uint64_t AddTNodeEvent();
		uint64_t AddTNodeAnimation();
		uint64_t AddTNodeEnd();
		uint64_t AddTNodeBegin();
		uint64_t AddEpisodeNode(uint64_t _id);
		uint64_t AddTNodeCharacterInOut();
		uint64_t AddEpisodeNode(uint64_t _id, int _in, int _out);
		uint64_t AddEpisodeNode(Episode _episode);
		uint64_t AddEpisodeNode(std::shared_ptr<Episode> _episode);
	};

	inline uint64_t teller::TNodeManager::MakeUID_ui64t() 
	{
		return ui64distr(engine);
	}
}