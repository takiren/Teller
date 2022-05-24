#include"NodeEditor.h"

using namespace Teller;

void SequenceEditor::AddNode(std::shared_ptr<NodeBase> node)
{
	sequenceTree.AddNode(node);
}

void Teller::SequenceEditor::RemoveNode()
{
}
