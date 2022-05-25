#pragma once
#include"Core.h"
#include "Game.h"
#include"Asset.h"
#include "cinder/Vector.h"
#include"Animation.h"

using namespace ci;
using namespace ci::app;

namespace Teller {

	class Agent :public ModuleCore
	{
	protected:
		cinder::vec3 position;
		cinder::vec3 scale;
		cinder::vec3 rotation;

		std::vector<std::shared_ptr<ModuleCore>> animations;

	public:
		Agent():ModuleCore(){};
		~Agent(){};
		std::weak_ptr<ModuleCore>& parentScene =
			ModuleCore::parent;

		void Tick() override;
		void Move();
		void Scale();
		void Rotate();
		void MessageHandler();
		void SetAnimation();
	};

	class Character :public Agent {
	private:
		std::unique_ptr<Sprite> csprite;
	public:
		Character(std::unique_ptr<Sprite> _sprite):Agent(),csprite(std::move(_sprite)){};
		void Tick() override;
		void SetSprite();
		void Update();
		void GetDraw();
	};
}

