#include "Scene.h"

using namespace teller;

void teller::SceneModule::Tick(float& deltaTime)
{
	// �t���[���o�b�t�@�[��FBO�ɐݒ�B
	//mFbo_->bindFramebuffer();
	gl::color(Color::black());
	gl::drawSolidRect(Rectf(0, 0, 1280, 720));
	gl::color(Color::white());
	for (auto& e : agents_) {
		e->Tick();
	}
	// �t���[���o�b�t�@�[��߂��ĕ`��B
	//gl::setMatricesWindow(ci::app::getWindowSize());
	//gl::draw(mFbo_->getColorTexture(),vec2(0,0));
}

void teller::SceneModule::Update(float& deltaTime)
{

}

void teller::SceneModule::Draw()
{
	gl::clear(Color::black());
	gl::color(Color::white());

}

void teller::SceneModule::AddAgent(AgentRef _agent)
{
	agents_.push_back(_agent);
}

void teller::SceneModule::SetArea(ci::Area _dest)
{
	destRect = _dest;
}
