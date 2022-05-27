#pragma once
#include"Core.h"
#include"ModuleCore.h"
#include"Asset.h"
#include"Episode.h"
#include"Animation.h"

using namespace ci;
using namespace ci::app;
using namespace Teller;

namespace Teller {
	class Agent :public ModuleCore
	{
	protected:
		::vec2 size_; //このエージェントの占める画面上の大きさ(ピクセル数)
		::vec2 position_; //画面上の位置
		::vec2 scale_;
		::vec2 rotation_;
		std::function<void(vec2&, vec2&, vec2&)> animatorCallBack_;
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

		void AttachAnimator(std::function<void(vec2&, vec2&, vec2&)>& _animatorCallBack);
		void AnimateInternal();
		virtual void Move();
		virtual void Scale();
		virtual void Rotate();
		virtual void MessageHandler();
		virtual void SetAnimation();
	};

	class Character :public Agent {
	private:
		std::weak_ptr<Sprite> sprite_;
	public:
		Character(std::shared_ptr<Sprite> _sprite) :Agent(), sprite_(_sprite) {};
		void Tick() override;
		void SetSprite();
		void Update() override;
		void GetDraw();
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
		void Update() override;
	};
}

