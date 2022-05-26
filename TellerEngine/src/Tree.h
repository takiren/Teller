#pragma once
#include"Core.h"
#include"ModuleCore.h"

namespace Teller {
	class Episode;
	enum class Node_TYPE {
		BLANK,
		EPISODE,
		EVENT,
		BRANCH
	};

	enum class Socket_TYPE {
		Delegate,
		BOOL,
		INT,
		OPTION
	};

	class SocketBase;

	class NodeBase :public std::enable_shared_from_this<NodeBase> {
	protected:
		int ID_ = 0;
		std::string description;
		ImVec2 position, size;
		Node_TYPE type;
	public:
		std::vector<std::shared_ptr<SocketBase>> inputSockets;
		std::vector<std::shared_ptr<SocketBase>> outputSockets;
		std::vector<std::shared_ptr<SocketBase>> sockets;
		std::vector<std::shared_ptr<NodeBase>> children;
		std::vector<std::weak_ptr<NodeBase>> parents;

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
}

