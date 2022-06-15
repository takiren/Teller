#pragma once
#include<vector>
#include<memory>
#include<string>
#include<map>
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

	class SocketBase;

	class NodeBase :public std::enable_shared_from_this<NodeBase> {
	protected:
		int ID_ = 0;
		std::string description;
		ImVec2 position, size;
		Node_TYPE type;
		std::vector<std::shared_ptr<SocketBase>> inputSockets;
		std::vector<std::shared_ptr<SocketBase>> outputSockets;
		std::vector<std::shared_ptr<SocketBase>> sockets;
	public:
		std::vector<std::weak_ptr<NodeBase>> parents;
		std::vector<std::shared_ptr<NodeBase>> children;

		NodeBase(int key) :ID_(key) {};
		~NodeBase() = default;
		int GetID() const { return ID_; }

		void AddSocket(int socketID, Socket_TYPE socketType);
		void AddChild(std::shared_ptr<NodeBase> child);
		void RemoveChild(int key);
		void SearchChild(int key);//子ノードのみ。
		void SearchChildDeeply(int key); //ノード端まで検索
		std::shared_ptr<NodeBase> Clone();
		bool IsEnd();//終端か判定
	};

	class NodeBlank :public NodeBase {
	private:
		std::weak_ptr<Episode> episode;
	public:
		NodeBlank(int key) :
			NodeBase(key)
		{};
		NodeBlank(int key, const std::shared_ptr<Episode>&& ptrEpisode) :
			NodeBase(key),
			episode(ptrEpisode) {};
		~NodeBlank() = default;
	};

	class NodeBegin :public NodeBase {
	private:
	public:
	};

	class NodeEpisode :public NodeBase {
	private:
		std::weak_ptr<Episode> episode;
	public:
		NodeEpisode(int key) :NodeBase(key) {};
		NodeEpisode(int key, const std::shared_ptr<Episode>&& ptrEpisode) :
			NodeBase(key),
			episode(ptrEpisode) {};
		~NodeEpisode() = default;
	};

	class SocketBase :public NodeBase {
	public:

		SocketBase(int key) :NodeBase(key) {};
		virtual ~SocketBase() = default;
	};

	class SocketTimeLine :public SocketBase {
	public:
		SocketTimeLine(int key) :SocketBase(key) {};
	};

	class SocketBool :public SocketBase {
	private:
		bool bTrue;
	public:
		SocketBool(int key) :SocketBase(key), bTrue(false) {};
	};

	class SocketInt :public SocketBase {
	private:
	public:

	};

	class SocketValue :public NodeBase {

	};

	class NodeGroup :public NodeBase {
	private:
	public:
	};

	class TreeClass {
	private:
		std::vector<std::shared_ptr<NodeBase>> nodes;
	public:
		void AddNode(const std::shared_ptr<NodeBase> node);

		void RemoveNode(int key);
	};
	//前方宣言
	class TNodeCore;

	struct TSocketCore {
		uint64_t ID_;
		std::weak_ptr<TNodeCore> parentTNode;
		std::vector<uint64_t> targetSocketsID_;
		Socket_TYPE type_;
		TSocketCore() = delete;

		TSocketCore(Socket_TYPE _type) :
			type_(_type),
			ID_((uint64_t)this)
		{};

		bool AddTarget(uint64_t _target);
	};


	struct TSocketInput :TSocketCore {
	};

	struct TSocketOutput :TSocketCore {
	};



	template<class>
	class TNodeTemplate {
	private:
	public:
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
		std::vector<std::shared_ptr<TSocketCore>> socketsInput;
		std::vector<std::shared_ptr<TSocketCore>> socketsOutput;

		TNodeCore() = delete;
		TNodeCore(Node_TYPE _type) :TNodeCore(_type, "", vec2(100, 50), vec2(100, 50)) {};
		TNodeCore(Node_TYPE _type, std::string _title, vec2 _pos, vec2 _size) :
			ID_((uint64_t)this),
			type_(_type),
			title_(_title),
			description(""),
			pos_(_pos),
			size_(_size),
			eventID_(-1),
			episodeID_(-1)
		{};

		void SetDesciption(std::string _description);
		void AddInputSocket(Socket_TYPE _scktType);

		void AddOutPutSocket(Socket_TYPE _scktType);
	};

	//選択肢用ノード。
	class TNodeBranch :public TNodeCore {
	public:
		TNodeBranch(vec2 _pos) :TNodeBranch("", _pos) {};
		TNodeBranch(std::string _title, vec2 _pos, vec2 _size = vec2(100, 50)) :
			TNodeCore(Node_TYPE::BRANCH, _title, _pos, _size) {
			AddInputSocket(Socket_TYPE::FLOW);
			AddOutPutSocket(Socket_TYPE::FLOW);
			AddOutPutSocket(Socket_TYPE::FLOW);
		};
		~TNodeBranch() = default;
	};

	class TNodeAnimation :public TNodeCore {
	public:

	};

	class TNodeEvent :TNodeCore {

	};

	class TNodeSceneChange :public TNodeCore {
	public:

	};

	struct LinkInfo
	{
		ed::LinkId Id;
		ed::PinId  InputId;
		ed::PinId  OutputId;
	};
	//ノード管理クラス
	class TNodeManager {
		std::unique_ptr<TNodeCore> MakeTNode(Node_TYPE _type);
		std::weak_ptr<Episode> episode_;
	public:
		TNodeManager() = default;

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
}

