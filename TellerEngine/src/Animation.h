#pragma once
#include<vector>
#include<memory>
#include<functional>
#include"cinder/Cinder.h"
#include"cinder/gl/gl.h"

namespace Teller {
	using namespace ci;
	
	class Animator {
	private:
	public:
		void Animate(std::function<void(vec2&, vec2&, vec2&)>&);
	};
}