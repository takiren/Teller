#include "Scene.h"

using namespace Teller;

void Teller::SceneModule::Tick(float& deltaTime)
{
	ModuleCore::Tick(deltaTime);
	// レンダーバッファーをFBOに設定。
	//mFbo_->bindFramebuffer();

	for (auto& e : agents_) {
		e->Tick();
	}
	//レンダーバッファーを戻して描画。
	//mFbo_->unbindFramebuffer();
	//gl::draw(mFbo_.get()->getColorTexture());
}

void Teller::SceneModule::AddAgent(std::shared_ptr<AgentCore> _agent)
{
	agents_.push_back(_agent);
}
