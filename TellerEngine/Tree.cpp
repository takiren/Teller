#include "Tree.h"

//Node実装
//SetParent(Node)は提供しない。AddChildですべて行ってもらう。

namespace Teller {

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
		child->parents.push_back(this->shared_from_this());
	}

	std::shared_ptr<NodeBase> NodeBase::Clone()
	{
		return this->shared_from_this();
	}

	bool NodeBase::IsEnd()
	{
		return children.empty();
	}

	void TreeClass::AddNode(const std::shared_ptr<NodeBase> node)
	{
		nodes.push_back(node->Clone());
	}


}