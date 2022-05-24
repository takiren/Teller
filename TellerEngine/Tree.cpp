#include "Tree.h"

//NodeÀ‘•
//SetParent(Node)‚Í’ñ‹Ÿ‚µ‚È‚¢BAddChild‚Å‚·‚×‚Äs‚Á‚Ä‚à‚ç‚¤B

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