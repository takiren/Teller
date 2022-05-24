#pragma once
#include "Game.h"
#include "cinder/Vector.h"

namespace Teller {
	
	class Agent :public ModuleCore
	{
	protected:
		cinder::vec3 position;
		cinder::vec3 scale;
		cinder::vec3 rotation;

	public:
		Agent() = default;
		virtual ~Agent() = default;
		virtual void Move();
		virtual void Scale();
		virtual void Rotate();
		virtual void MessageHandler();
	};

	class Character :public Agent {
	public:
		void Tick();
		void Update();
	};
}

