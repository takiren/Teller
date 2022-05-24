#pragma once
#include"Core.h"
#include"Episode.h"

namespace Teller {
	namespace Tree {
		enum class Node_TYPE {
			BLANK,
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
			Node_TYPE type;
		public:
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
			bool IsEnd();//終端か判定
		};

		template<class T>
		class NodeBlank :public NodeBase {
		private:
			std::unique_ptr<Episode> episode;
		public:
			NodeBlank(int key) :NodeBase(key) {};
			~NodeBlank() = default;
		};

		class NodeEpisode :public NodeBase {
		private:
			std::unique_ptr<Episode> episode;
		public:
			NodeEpisode(int key) :NodeBase(key) {};
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

	}

}

