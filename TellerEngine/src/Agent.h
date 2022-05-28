#pragma once
#include"cinder/Cinder.h"
#include"cinder/gl/gl.h"

#include"Asset.h"
#include"Episode.h"

using namespace ci;

namespace Teller {

	class Agent
	{
	protected:
		::vec2 size_; //このエージェントの占める画面上の大きさ(ピクセル数)
		::vec2 position_; //画面上の位置
		::vec2 scale_;
		::vec2 rotation_;

		//Animatorクラス用変数
		std::function<void(vec2&, vec2&, vec2&)> animatorCallBack_;
		std::map<int, std::function<void()>> animatorCallBackMap_;
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

		virtual void Animate(vec2 _dpos, vec2 _drot, vec2 _dscale);
		virtual void Scale();
		virtual void Rotate();
		virtual void MessageHandler();
		virtual void SetAnimation();
		virtual void Tick();

		//virtual void AddAnimator(int key,std::shared_ptr<Animator> _animator);
	};

	class Character :public Agent {
	private:
		std::weak_ptr<Sprite> sprite_;
	public:
		Character(std::shared_ptr<Sprite> _sprite) :Agent(), sprite_(_sprite) {};
		void Tick() override;
		void SetSprite();
		void GetDraw();
	};

	class RectAgent :public Agent {
	private:
	public:
		void Tick() override;
	};

	class Text :public Agent {
	private:
		void Initialize();
		std::string path_;
		std::vector<Episode> episodes;
		int count;
		int currentLine;
		std::string currentText;
	public:
		Text(std::string path) :
			Agent(),
			count(0),
			currentLine(0),
			currentText(""),
			path_(path)
		{
			Initialize();
		};
		void Tick() override;
	};
}

