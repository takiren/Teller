#include "Tree.h"

//NodeÀ‘•
//SetParent(Node)‚Í’ñ‹Ÿ‚µ‚È‚¢BAddChild‚Å‚·‚×‚Äs‚Á‚Ä‚à‚ç‚¤B

namespace teller {


	void TNodeCore::SetDesciption(std::string _description)
	{
	}

	void TNodeCore::AddInputSocket(Socket_TYPE _scktType, uint64_t _id)
	{
		auto sckt = std::make_shared<TSocketCore>(_scktType, _id);
		sckt->parentTNode = shared_from_this();
		socketsInput.push_back(sckt);
	}
	void TNodeCore::AddOutPutSocket(Socket_TYPE _scktType, uint64_t _id)
	{
		auto sckt = std::make_unique<TSocketCore>(_scktType, _id);
		sckt->parentTNode = shared_from_this();
		socketsOutput.push_back(std::move(sckt));
	}

	std::vector<TLinkInfo> TNodeManager::GetLinkVector()
	{
		auto links = std::vector<TLinkInfo>();
		int l = 1;
		for (auto iter = nodes.begin(); iter != nodes.end(); ++iter) {
			l += iter->second->socketsInput.size() + iter->second->socketsOutput.size();
		}

		auto mx = std::make_unique<Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic>>();

		return std::vector<TLinkInfo>();
	}

	std::shared_ptr<TSocketCore> TNodeManager::SearchSocket(TSocketID _ID)
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
		auto n = std::make_shared<TNodeCore>(Node_TYPE::BRANCH, "Branch", MakeUID_ui64t());
		n->title_ = "Branch node";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		nodes[n->ID_] = n;
		return n->ID_;
	}

	uint64_t TNodeManager::AddTNodeSceneChange()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::SCENECHANGE, "Scenechange ", MakeUID_ui64t());
		n->title_ = "Scene change.";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		nodes[n->ID_] = n;
		return n->ID_;

	}

	uint64_t TNodeManager::AddTNodeEvent()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::EVENT, "Event", MakeUID_ui64t());
		n->title_ = "Event node.";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		printf("%d", (uint64_t)&n);
		nodes[n->ID_] = n;
		return n->ID_;

	}

	uint64_t TNodeManager::AddTNodeAnimation()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::ANIMATION, "animation", MakeUID_ui64t());
		n->title_ = "Animation node.";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		printf("%d", (uint64_t)&n);
		nodes[n->ID_] = n;
		return n->ID_;

	}

	uint64_t TNodeManager::AddTNodeBegin()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::BEGINEPISODE, "Begin", MakeUID_ui64t());
		n->title_ = "Episode begin.";
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		std::cout << n->ID_ << std::endl;

		beginNode_ = n;
		return n->ID_;

	}

	uint64_t TNodeManager::AddEpisodeNode(uint64_t _id)
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::EPISODE, "Episode", MakeUID_ui64t());

		n->title_ = "Episode";
		n->episodeID_ = _id;
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		auto nid = n->ID_;

		nodes[n->ID_] = n;
		return nid;
	}

	uint64_t TNodeManager::AddTNodeCharacterInOut()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::CHARACTER_IN_OUT,"CharacterIn", MakeUID_ui64t());
		n->title_ = "Character In Out";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		n->AddOutPutSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
		auto nid = n->ID_;
		nodes[n->ID_] = n;

		return nid;
	}

	uint64_t TNodeManager::AddEpisodeNode(std::shared_ptr<Episode> _episode)
	{
		return -1;
	}


	uint64_t TNodeManager::AddTNodeEnd()
	{
		auto n = std::make_shared<TNodeCore>(Node_TYPE::ENDEPISODE,"End", MakeUID_ui64t());
		n->title_ = "Episode endF";
		n->AddInputSocket(Socket_TYPE::FLOW, MakeUID_ui64t());
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