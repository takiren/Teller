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

		class Socket :public std::enable_shared_from_this<Socket> {
		private:
			Socket_TYPE type;
			int ID;
		public:
			Socket(Socket_TYPE socketType) : type(socketType) {};
		};

		class Node :public std::enable_shared_from_this<Node> {
		private:
			int uniqueKey;
			Node_TYPE type;
			std::vector<std::shared_ptr<Node>> children;
			std::weak_ptr<Node> parent;
		public:
			//�R���X�g���N�^
			Node(int key)
				:uniqueKey(key),
				type(Node_TYPE::BLANK) {};
			Node(int key, Node_TYPE nodeType)
				:uniqueKey(key),
				type(nodeType) {};

			//�f�X�g���N�^
			~Node() = default;
			void AddChild(const std::shared_ptr<Node> child);
			void AddSocket(Socket_TYPE socketType);
			void RemoveSocket(int socketID);
			void RemoveChild(int key);
			void SearchChild(int key);//�q�m�[�h�̂݁B
			void SearchChildDeeply(int key); //�m�[�h�[�܂Ō���
			void IsEnd();//�I�[������
		};

		class NodeGroup :
			public std::enable_shared_from_this<NodeGroup>,
			public Node
		{
		private:
			std::string description;
		public:
			NodeGroup() = default;
			virtual ~NodeGroup() = default;
		};

	}

}

