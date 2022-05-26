#pragma once
#include"Core.h"

namespace Teller {
	class NodeBase;
	class TreeClass;

	class NodeEditor {
	private:
	public:
		NodeEditor() {};
		virtual void AddNode(std::shared_ptr<NodeBase> node);
		virtual void RemoveNode();
		void Update();
	};

}