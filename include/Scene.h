#ifndef _TELLER_H_SCENE_
#define _TELLER_H_SCENE_

#include<vector>
#include<algorithm>
#include<map>
#include<memory>

#include"cinder/gl/gl.h"
#include"cinder/gl/Fbo.h"

#include"Agent.h"
#include"TAgent.h"

namespace teller {
	using namespace ci;

	using AgentRef = std::shared_ptr<AgentCore>;

	template<class KEY, class DATA>
	using HashMap = std::unordered_map<KEY, DATA>;

	class SceneModule
	{
	private:
		std::vector<AgentRef> agents_;
		HashMap<std::string, TAgentRef> tAgents_;

		gl::FboRef mFbo_;
		Rectf destRect;
		vec2 destTopLeft;
		vec2 destSize;
		vec2 destScaleFactor;
	public:
		SceneModule() :
			destSize(vec2(800, 600)),
			mFbo_(gl::Fbo::create(1920, 1080, true)),
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

		SceneModule(const SceneModule&) = delete;
		SceneModule& operator=(const SceneModule&) = delete;

		SceneModule& operator=(SceneModule&&) = default;

		void Tick(float& deltaTime);

		void Update(float& deltaTime);
		void Draw();

		void AddAgent(AgentRef _agent);
		void SetArea(ci::Area _dest);

		//It returns TAgentRef. If it's iter::end, the ret-value is nullptr.
		inline TAgentRef FindTAgent(std::string _name) {
			auto& val = tAgents_.find(_name);
			return val == tAgents_.end() ? val->second : nullptr;
		};

		inline void AddTAgent(TAgentRef _tAgent) {};
		inline void AddTAgent(TAgentRef&& _tAgent) {};
	};
}
#endif // !_TELLER_H_SCENE_
