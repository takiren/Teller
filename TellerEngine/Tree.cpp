#include "Tree.h"

//Node実装
//SetParent(Node)は提供しない。AddChildですべて行ってもらう。

namespace Teller {
	namespace Tree {
		void NodeBase::AddSocket(int socketID, Socket_TYPE socketType)
		{
			switch (socketType)
			{
			case Socket_TYPE::Delegate:

				break;
			default:
				break;
			}
		}
		void NodeBase::AddChild(const std::shared_ptr<NodeBase> child)
		{
			children.push_back(child);
			child->parents.push_back( this->shared_from_this());
		}
		bool NodeBase::IsEnd()
		{
			return children.empty();
		}
	}
}