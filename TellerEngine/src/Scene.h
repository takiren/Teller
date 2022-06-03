#pragma once
#include<vector>
#include<memory>

#include"cinder/gl/gl.h"
#include"cinder/gl/Fbo.h"
#include"ModuleCore.h"
#include"Agent.h"

namespace Teller {
	using namespace ci;
	class SceneModule :	public ModuleCore
	{
	private:
		std::vector<std::shared_ptr<AgentCore>> agents_;
		gl::FboRef mFbo_;
		Rectf destArea;
		vec2 destTopLeft;
		vec2 destSize;
		vec2 destScaleFactor;
	public:
		SceneModule() :ModuleCore(),
			destSize(vec2(1920,1080)),
			mFbo_(gl::Fbo::create(1920,1080)),
			destTopLeft(vec2(0,0)),
			destScaleFactor(0.6),
			destArea(Rectf(ci::Area(destTopLeft,destSize*destScaleFactor)))
		{};
		~SceneModule(){};
		void Tick(float& deltaTime) override;
		void AddAgent(std::shared_ptr<AgentCore> _agent);
	};
}
