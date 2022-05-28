#pragma once
#include"Tree.h"
#include<memory>

namespace Teller {
	class NodeEditor {
	private:
	public:
		NodeEditor() {};
		virtual void AddNode(std::shared_ptr<NodeBase> node);
		virtual void RemoveNode();
		void Update();
	};

}