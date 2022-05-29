#pragma once
#include"cinder/Cinder.h"
#include"cinder/gl/gl.h"
#include"cinder/CinderImGui.h"
#include<string>

#include"Asset.h"
#include"Episode.h"

using namespace ci;

namespace Teller {
	class AgentCore {
	private:
	protected:
		float deltaTime_;
	public:
		AgentCore() :deltaTime_(0.0f) {};
		virtual ~AgentCore() = default;
		virtual void Tick();
		void SetDeltaTime(float _deltaTime);
	};

	template<class... Args>
	class Agent :public AgentCore
	{
	protected:
		::vec2 position_; //画面上の位置
		::vec2 rotation_;
		::vec2 scale_;
		::vec2 size_; //このエージェントの占める画面上の大きさ(ピクセル数)
		//Animatorクラス用変数

	public:
		Agent() :
			size_(vec2(1)),
			position_(vec2(0, 0)),
			scale_(vec2(1, 1)),
			rotation_(vec2(0, 0))
		{};

		Agent(ci::vec2 _position, ci::vec2 _scale, ci::vec2 _rotation) :
			position_(_position),
			scale_(_scale),
			rotation_(_rotation)
		{};

		Agent(const Agent&) = delete;
		Agent& operator=(const Agent&) = delete;

		Agent& operator=(Agent&&) = default;

		/*virtual void AttachAnimator(std::shared_ptr<Animator>&& _animator,int key);
		virtual void AnimateInternal(int key, float factor);
		virtual void Move();*/

		virtual void CallBackListener(Args... args);

		//virtual void AddAnimator(int key,std::shared_ptr<Animator> _animator);
	};

	template<class ...Args>
	inline void Agent<Args...>::CallBackListener(Args ...)
	{
	}

	class Character :public Agent<vec2, vec2, vec2> {
	private:
		std::weak_ptr<Sprite> sprite_;
	public:
		void Tick() override;
		void CallBackListener(vec2 _pos, vec2 _rot, vec2 _scale) override;

	};

	class RectAgent :public Agent<vec2, vec2, vec2> {
	private:
	public:
		void Tick() override;
		void CallBackListener(vec2 _pos, vec2 _rot, vec2 _scale) override;
	};

	class MainTextArea :public Agent<std::string, std::string> {
	private:
		std::string speaker_;
		std::string text_;
	public:
		MainTextArea() :Agent(), text_(""), speaker_("") {};
		void Tick() override;

		void CallBackListener(std::string _speaker, std::string _text);
	};


}

