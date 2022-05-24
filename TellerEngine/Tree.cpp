#include "Tree.h"

//NodeÀ‘•
//SetParent(Node)‚Í’ñ‹Ÿ‚µ‚È‚¢BAddChild‚Å‚·‚×‚Äs‚Á‚Ä‚à‚ç‚¤B

namespace Teller {
	namespace Tree {
		
		void NodeBase::AddChild(const std::shared_ptr<NodeBase> child)
		{
			this->children.push_back(child);
			child->parent = shared_from_this();
		}

		void NodeBase::AddSocket(int socketID, Socket_TYPE socketType)
		{
			auto newSocket = std::make_shared<Socket>(socketID, socketType);
			this->sockets.push_back(newSocket);
		}

		void NodeBase::AppendSocket(Socket socket)
		{

		}

		
		void NodeBase::RemoveChild(int key)
		{
		}

		void NodeBase::SearchChild(int key)
		{
		}
		
		void NodeBase::SearchChildDeeply(int key)
		{
		}
		void Socket::Attach(std::shared_ptr<Socket> target)
		{
			this->targetSockets.push_back(target);
			target->previousSockets.push_back(shared_from_this());
		}
		void Socket::Dettach(std::shared_ptr<Socket> target)
		{
			this->targetSockets.push_back(target);
			target->previousSockets.push_back(shared_from_this());
		}
	}
}