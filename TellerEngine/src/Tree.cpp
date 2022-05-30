#include "Tree.h"

//NodeÀ‘•
//SetParent(Node)‚Í’ñ‹Ÿ‚µ‚È‚¢BAddChild‚Å‚·‚×‚Äs‚Á‚Ä‚à‚ç‚¤B

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


	void TNodeCore::SetDesciption(std::string _description)
	{
	}

	void TNodeCore::AddInputSocket(Socket_TYPE _scktType)
	{
		auto sckt = std::make_unique<TSocketCore>(_scktType);
		sckt->parentTNode = this->shared_from_this();
		socketsInput.push_back(std::move(sckt));
	}
	void TNodeCore::AddOutPutSocket(Socket_TYPE _scktType)
	{
		auto sckt = std::make_unique<TSocketCore>(_scktType);
		sckt->parentTNode = this->shared_from_this();
		socketsOutput.push_back(std::move(sckt));
	}

	void TNodeManager::AddTNode(Node_TYPE _type) {

	}


	std::vector<TNodeManager::LinkInfo> TNodeManager::MakeLinkVector()
	{
		auto links = std::vector<LinkInfo>();
		int l = 1;
		for (auto iter = nodes.begin(); iter != nodes.end(); ++iter) {
			l += iter->second->socketsInput.size() + iter->second->socketsOutput.size();
		}

		auto mx = std::make_unique<Eigen::Matrix<int,Eigen::Dynamic,Eigen::Dynamic>>();

		return std::vector<LinkInfo>();
	}

}