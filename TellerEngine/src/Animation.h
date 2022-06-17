#pragma once
#include<functional>
#include<map>
#include"cinder/Cinder.h"
#include"cinder/gl/gl.h"
#include"cinder/Timer.h"

#include"Agent.h"
#include"Episode.h"

namespace teller {
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
		//ベクトルを与える
		//A letter "d" means diff
		vec2 dpos_;
		vec2 drot_;
		vec2 dscale_;

		//TODO:Delete this valiable.
		std::weak_ptr<Agent<Args...>> targetAgent;

		float deltaTime_;
		using AnimatorTargetFunc = std::function<void(Args...)>;
		AnimatorTargetFunc callback_;

	public:
		Animator() :
			deltaTime_(1),
			dpos_(vec2(0)),
			drot_(vec2(0)),
			dscale_(1) {};

		Animator(std::shared_ptr<Agent<Args...>> _agent) :
			deltaTime_(1),
			dpos_(vec2(0)),
			drot_(vec2(0)),
			dscale_(vec2(0)),
			callback_([&_agent](Args... args) {_agent.get()->CallBackListener(args...); })
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
		//TODO:なぜ _agent->CallBackListner(args...)を渡すとnullptrが返されるのかわからない
		//多分キャプチャする前に寿命が切れてるからだけど要検証。
		callback_ = [&](Args... args) {targetAgent.lock()->CallBackListener(args...); };

		//これは動かない
		//callback_ = [&](Args... args) {_agent->CallBackListener(args...); };
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
		std::string speaker_;
		std::string text_;
		int count_;
		void Initialize();
	public:
		TextChanger() :
			Animator(),
			currentline(1),
			count_(0),
			speaker_(""),
			text_("")
		{};

		void Update() override;
		//次の行へ。
		void Next();

		void SetText(std::string _speaker, std::string _text);
	};


	class CharacterAppearanceChanger :public Animator<vec2,vec2,vec2,std::string> {
	private:
	public:
		CharacterAppearanceChanger() :Animator() {};
		void Change(std::string _key) { callback_(vec2(0), vec2(0), vec2(0),_key); };
	};

	class CharacterAnimator :public Animator<vec2, vec2, vec2, std::string> {
	private:
		//スプライトの名前設定
		std::string sprite_;
	public:
		CharacterAnimator() :Animator() {};
		void Update() override;

		void SetSprite(std::string _sprite) { sprite_ = _sprite; };
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