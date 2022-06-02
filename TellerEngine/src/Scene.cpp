#include "Scene.h"

using namespace Teller;

void Teller::SceneModule::Tick(float& deltaTime)
{
	ModuleCore::Tick(deltaTime);
	// �����_�[�o�b�t�@�[��FBO�ɐݒ�B
	//mFbo_->bindFramebuffer();

	for (auto& e : agents_) {
		e->Tick();
	}
	//�����_�[�o�b�t�@�[��߂��ĕ`��B
	//mFbo_->unbindFramebuffer();
	//gl::draw(mFbo_.get()->getColorTexture());
}

void Teller::SceneModule::AddAgent(std::shared_ptr<AgentCore> _agent)
{
	agents_.push_back(_agent);
}
