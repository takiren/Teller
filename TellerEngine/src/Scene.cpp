#include "Scene.h"

using namespace teller;

void teller::SceneModule::Tick(float& deltaTime)
{
	ModuleCore::Tick(deltaTime);
	// フレームバッファーをFBOに設定。
	//mFbo_->bindFramebuffer();
	gl::color(Color::black());
	gl::drawSolidRect(Rectf(0, 0, 1280, 720));
	gl::color(Color::white());
	for (auto& e : agents_) {
		e->Tick();
	}
	// フレームバッファーを戻して描画。
	//gl::setMatricesWindow(ci::app::getWindowSize());
	//gl::draw(mFbo_->getColorTexture(),vec2(0,0));
}

void teller::SceneModule::AddAgent(std::shared_ptr<AgentCore> _agent)
{
	agents_.push_back(_agent);
}

void teller::SceneModule::SetArea(ci::Area _dest)
{
	destRect = _dest;
}
