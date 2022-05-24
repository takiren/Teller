#pragma once
#include"Core.h"
#include"Tree.h"
#include"Editor.h"

namespace Teller {
	class NodeEditor {
	private:
	public:
		NodeEditor() {};
		virtual void AddNode(std::shared_ptr<NodeBase> node);
		virtual void RemoveNode();
		void Update();
	};

	class SequenceEditor :public NodeEditor {
	private:
		TreeClass sequenceTree;
	public:
		SequenceEditor() :NodeEditor() {};
		void AddNode(std::shared_ptr<NodeBase> node) override;
		void RemoveNode() override;
	};
}