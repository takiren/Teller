#pragma once
#include<functional>
#include<map>
#include"cinder/Cinder.h"
#include"cinder/gl/gl.h"
#include"cinder/Timer.h"

#include"Agent.h"
#include"Episode.h"

namespace Teller {
	using namespace ci;

	class AnimatorCore {
	protected:
		std::string uniqueName;
	public:
		AnimatorCore() :uniqueName("") {};
		AnimatorCore(std::string _name) :uniqueName(_name) {};
		virtual ~AnimatorCore() = default;
		virtual void Update();
	};

	template<class... Args>
	class Animator :public AnimatorCore {
	protected:
		//A letter "d" means diff
		vec2 dpos_;
		vec2 drot_;
		vec2 dscale_;
		std::weak_ptr<Agent<Args...>> targetAgent;
		float deltaTime_;
		using AnimatorTargetFunc = std::function<void(Args...)>;
		AnimatorTargetFunc callback_;

	public:
		Animator() :
			deltaTime_(1),
			dpos_(vec2(0)),
			drot_(vec2(0)),
			dscale_(1)
		{};

		Animator(const Animator&) = delete;
		Animator& operator=(const Animator&) = delete;

		Animator& operator=(Animator&&) = default;

		void SetDeltaTime(float _deltaTime);

		virtual void Update();

		void AttachToAgent(std::shared_ptr<Agent<Args...>> _agent);
	};

	template<class ...Args>
	inline void Animator<Args...>::SetDeltaTime(float _deltaTime)
	{
	}

	template<class ...Args>
	inline void Animator<Args...>::Update()
	{
	}

	template<class ...Args>
	inline void Animator<Args...>::AttachToAgent(std::shared_ptr<Agent<Args...>> _agent)
	{
		targetAgent = _agent;
		callback_ = [&](Args... args) {targetAgent.lock()->CallBackListener(args...); };
	}

	class Circular :public Animator<vec2, vec2, vec2> {
	private:
		float theta;
		Timer timer_;
		void Initialize();
	public:
		Circular() :Animator(), theta(0) { Initialize(); };
		void Update() override;
	};

	class Repeat :public Animator<vec2> {
	private:
		float theta;
	public:
		Repeat() :Animator(), theta(0) {};
		void Update() override;
	};

	class TextChanger :public Animator<std::string, std::string> {
	private:
		Timer timer_;
		std::unique_ptr<CSVLoader> csvData;
		int currentline;
		int count_;
		void Initialize();
	public:
		TextChanger() :Animator(), currentline(1), count_(0) { ; };
		void Update() override;
		//���̍s�ցB
		void Next();
		void LoadCSV(std::string path) {
			csvData = std::move(std::make_unique<CSVLoader>(path)); Initialize();
		};
	};

	class AnimationSequencer {
	private:
		std::string uniqueName;
		std::vector<std::unique_ptr<AnimatorCore>> animators;
	public:
		AnimationSequencer() = delete;
		AnimationSequencer(std::string _name) :uniqueName(_name) {};
		~AnimationSequencer() = default;

		AnimationSequencer(const AnimationSequencer&) = delete;
		AnimationSequencer& operator=(const AnimationSequencer&) = delete;

		AnimationSequencer& operator=(AnimationSequencer&&) = default;

		void Update();
		void AddAnimator(std::unique_ptr<AnimatorCore> _animator);
	};

}