#pragma once
#include"Core.h"

namespace Teller {
	namespace Tree {
		enum class Node_TYPE {
			BLANK,
			EVENT,
			BRANCH
		};

		enum class Socket_TYPE {
			TimeLine,
			BOOL,
			INT,
			OPTION
		};

		class Socket;

		class NodeBase :public std::enable_shared_from_this<NodeBase> {
		private:
			int ID_;
			Node_TYPE type;
		public:
			//コンストラクタ
			NodeBase() = default;
			NodeBase(int id)
				:ID_(id),
				type(Node_TYPE::BLANK) {};
			NodeBase(int id, Node_TYPE nodeType)
				:ID_(id),
				type(nodeType) {};

			//デストラクタ
			~NodeBase() = default;

			std::weak_ptr<NodeBase> parent;
			std::vector<std::shared_ptr<Socket>> sockets;
			std::vector<std::shared_ptr<NodeBase>> children;
			std::vector<std::weak_ptr<NodeBase>> parents;

			std::weak_ptr<NodeBase>& GetParent();
			int GetID() const { return ID_; }
			void AddChild(const std::shared_ptr<NodeBase> child);
			void AddSocket(int socketID, Socket_TYPE socketType);
			void AppendSocket(Socket socket);
			void RemoveSocket(int socketID);
			void RemoveChild(int key);
			void SearchChild(int key);//子ノードのみ。
			void SearchChildDeeply(int key); //ノード端まで検索
			void IsEnd();//終端か判定
		};

		class Node :public std::enable_shared_from_this<Node> {
		private:
		public:
			
		};

		class Socket :public std::enable_shared_from_this<Socket>
		{
		private:
			int ID;
			Socket_TYPE type;
		public:
			Socket(int id, Socket_TYPE socketType) :ID(id), type(socketType) {};
			std::vector<std::weak_ptr<Socket>> previousSockets;
			std::vector<std::shared_ptr<Socket>> targetSockets;
			void Attach(std::shared_ptr<Socket> target);
			void Dettach(std::shared_ptr<Socket> target);
		};


		class NodeGroup :
			public std::enable_shared_from_this<NodeGroup>,
			public NodeBase
		{
		private:
			std::string description;
		public:
			NodeGroup() = default;
			virtual ~NodeGroup() = default;
		};

	}

}

