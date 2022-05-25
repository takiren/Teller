#pragma once
#include "Game.h"
#include "cinder/Vector.h"
#include"Animation.h"

namespace Teller {

	class Agent :public ModuleCore
	{
	protected:
		cinder::vec3 position;
		cinder::vec3 scale;
		cinder::vec3 rotation;

		std::vector<std::shared_ptr<ModuleCore>>& animations =
			ModuleCore::children;

	public:
		Agent() = default;
		virtual ~Agent() = default;
		virtual void Move();
		virtual void Scale();
		virtual void Rotate();
		virtual void MessageHandler();
		virtual void SetAnimation();
	};

	class Character :public Agent {
	public:
		void Tick();
		void Update();
	};
}

