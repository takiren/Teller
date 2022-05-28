#pragma once
#include"cinder/Cinder.h"
#include"cinder/gl/gl.h"

namespace Teller {
	using namespace ci;

	class Animator {
	private:
	public:
		void Animate(vec2&, vec2&, vec2&);
	};
}