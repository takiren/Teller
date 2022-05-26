#pragma once
#include"Core.h"
#include"Asset.h"
#include"Agent.h"
#include"Episode.h"
#include"ModuleCore.h"
#include"TellerCore.h"
#include<string>

using namespace ci;
using namespace ci::app;

namespace Teller {
	class ModuleCore;
	class Sprite;
	class Episode;
	class TellerCore;
	class Agent;
	class Sprite;


	class Agent :public ModuleCore
	{
	protected:
		::vec3 size; //このエージェントの占める画面上の大きさ(ピクセル数)
		::vec3 position; //画面上の位置
		::vec3 scale;
		::vec3 rotation;
		//std::vector<std::shared_ptr<ModuleCore>> animations;

	public:
		Agent() :
			ModuleCore(),
			size(vec3(1)),
			position(vec3(0, 0, 0)),
			scale(vec3(1, 1, 1)),
			rotation(vec3(0, 0, 0))
		{};

		Agent(ci::vec3 _position, ci::vec3 _scale, ci::vec3 _rotation) :
			ModuleCore(),
			position(_position),
			scale(_scale),
			rotation(_rotation)
		{};

		virtual ~Agent() = default;
		std::weak_ptr<ModuleCore>& parentScene =
			ModuleCore::parent;
		void Move();
		void Scale();
		void Rotate();
		void MessageHandler();
		void SetAnimation();
	};

	class Character :public Agent {
	private:
		std::weak_ptr<Sprite> sprite_;
	public:
		Character(std::shared_ptr<Sprite> _sprite) :Agent(), sprite_(_sprite) {};
		void Tick() override;
		void SetSprite();
		void Update();
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

