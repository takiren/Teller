#pragma once
#include<vector>
#include<memory>

#include"cinder/gl/gl.h"
#include"cinder/gl/Fbo.h"

#include"Agent.h"

namespace teller {
	using namespace ci;

	class SceneModule
	{
	private:
		std::vector<std::shared_ptr<AgentCore>> agents_;
		gl::FboRef mFbo_;
		Rectf destRect;
		vec2 destTopLeft;
		vec2 destSize;
		vec2 destScaleFactor;
	public:
		SceneModule():
			destSize(vec2(800, 600)),
			mFbo_(gl::Fbo::create(1920, 1080,true)),
			destTopLeft(vec2(100, 100)),
			destScaleFactor(0.6),
			destRect(Rectf(ci::Area(vec2(0, 0), vec2(1280, 720))))
		{};
		SceneModule(ci::Area _screenArea) :
			destSize(vec2(1280, 720)),
			mFbo_(gl::Fbo::create(_screenArea.getWidth(), _screenArea.getHeight())),
			destTopLeft(vec2(0, 0)),
			destScaleFactor(0.6),
			destRect(
				Rectf(ci::Area(vec2(0, 0), vec2(_screenArea.getWidth(), _screenArea.getHeight())))
			)
		{};
		~SceneModule() {};
		void Tick(float& deltaTime);
		void AddAgent(std::shared_ptr<AgentCore> _agent);
		void SetArea(ci::Area _dest);
	};
}
