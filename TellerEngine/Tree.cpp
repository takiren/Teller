#include "Tree.h"

//Node実装
//SetParent(Node)は提供しない。AddChildですべて行ってもらう。

namespace Teller {
	namespace Tree {
		
		void Node::AddChild(const std::shared_ptr<Node> child)
		{
			this->children.push_back(child);
			child->parent = shared_from_this();
		}

		
		void Node::RemoveChild(int key)
		{
		}

		void Node::SearchChild(int key)
		{
		}
		
		void Node::SearchChildDeeply(int key)
		{
		}
	}
}