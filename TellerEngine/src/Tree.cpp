#include "Tree.h"

//Nodeé¿ëï
//SetParent(Node)ÇÕíÒãüÇµÇ»Ç¢ÅBAddChildÇ≈Ç∑Ç◊ÇƒçsÇ¡ÇƒÇ‡ÇÁÇ§ÅB

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
		auto sckt = std::make_shared<TSocketCore>(_scktType);
		sckt->parentTNode = shared_from_this();
		socketsInput.push_back(sckt);
	}
	void TNodeCore::AddOutPutSocket(Socket_TYPE _scktType)
	{
		auto sckt = std::make_unique<TSocketCore>(_scktType);
		sckt->parentTNode = shared_from_this();
		socketsOutput.push_back(std::move(sckt));
	}

	void TNodeManager::AddTNode(Node_TYPE _nodeType)
	{
		auto _node = MakeTNode(_nodeType);
		std::shared_ptr<TNodeCore> n = std::move(_node);
		nodes.emplace(_node->ID_, n);
		//nodes[_node->ID_] =std::move(_node);
	}

	std::unique_ptr<TNodeCore> TNodeManager::MakeTNode(Node_TYPE _type)
	{
		switch (_type)
		{
		case Teller::Node_TYPE::BLANK:
			return std::make_unique<TNodeBranch>(vec2(100, 100));
			break;
		case Teller::Node_TYPE::EPISODE:
			break;
		case Teller::Node_TYPE::EVENT:
			break;
		case Teller::Node_TYPE::BRANCH:
			return std::make_unique<TNodeBranch>(vec2(100, 100));
			break;
		default:
			break;
		}
		return std::unique_ptr<TNodeCore>();
	}

	std::vector<TNodeManager::LinkInfo> TNodeManager::GetLinkVector()
	{
		auto links = std::vector<LinkInfo>();
		int l = 1;
		for (auto iter = nodes.begin(); iter != nodes.end(); ++iter) {
			l += iter->second->socketsInput.size() + iter->second->socketsOutput.size();
		}

		auto mx = std::make_unique<Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic>>();

		return std::vector<LinkInfo>();
	}

	void TNodeManager::MakeLink(std::shared_ptr<TSocketCore> _from, uint64_t _dest)
	{
		_from->AddTarget(_dest);
	}

	void TNodeManager::MakeLink(uint64_t _from, uint64_t _dest)
	{
		auto sf = SearchSocket(_from);
		sf->AddTarget(_dest);
	}

	std::shared_ptr<TSocketCore> TNodeManager::SearchSocket(uint64_t _ID)
	{
		for (auto& e : nodes) {
			for (auto& ein : e.second->socketsInput) {
				if (ein.get()->ID_ == _ID) return ein;
			}
			for (auto& eout : e.second->socketsOutput) {
				if (eout.get()->ID_ == _ID) return eout;
			}
		}

		return nullptr;
	}

	uint64_t TNodeManager::AddTNodeBranch()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::BRANCH);
		n->title_ = "Branch node";
		n->AddInputSocket(Socket_TYPE::FLOW);
		n->AddOutPutSocket(Socket_TYPE::FLOW);
		n->AddOutPutSocket(Socket_TYPE::FLOW);
		nodes[n->ID_] = n;
		return n->ID_;
	}

	uint64_t TNodeManager::AddTNodeSceneChange()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::SCENECHANGE);
		n->title_ = "Scene change.";
		n->AddInputSocket(Socket_TYPE::FLOW);
		n->AddOutPutSocket(Socket_TYPE::FLOW);
		nodes[n->ID_] = n;
		return n->ID_;

	}

	uint64_t TNodeManager::AddTNodeEvent()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::EVENT);
		n->title_ = "Event node.";
		n->AddInputSocket(Socket_TYPE::FLOW);
		n->AddOutPutSocket(Socket_TYPE::FLOW);
		printf("%d", (uint64_t)&n);
		nodes[n->ID_] = n;
		return n->ID_;

	}

	uint64_t TNodeManager::AddTNodeAnimation()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::ANIMATION);
		n->title_ = "Animation node.";
		n->AddInputSocket(Socket_TYPE::FLOW);
		n->AddOutPutSocket(Socket_TYPE::FLOW);
		printf("%d", (uint64_t)&n);
		nodes[n->ID_] = n;
		return n->ID_;

	}


	uint64_t TNodeManager::AddTNodeBegin()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::BEGINEPISODE);
		n->title_ = "Episode begin.";
		n->AddOutPutSocket(Socket_TYPE::FLOW);
		std::cout << n->ID_ << std::endl;

		beginNode_ = n;
		return n->ID_;

	}


	uint64_t TNodeManager::AddTNodeEnd()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::ENDEPISODE);
		n->title_ = "Episode endF";
		n->AddInputSocket(Socket_TYPE::FLOW);
		endNode_ = n;
		return n->ID_;

	}

	bool TSocketCore::AddTarget(uint64_t _target)
	{
		targetSocketsID_.push_back(_target);
		return true;
		/*if (_target->parentTNode.lock().get() == this->parentTNode.lock().get()) {
			return false;
		}
		if (_target->kind_ == this->kind_) return false;
		targetSockets_.push_back(_target);
		return true;*/
	}

}