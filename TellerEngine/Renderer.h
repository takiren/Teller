#pragma once
#include"Core.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

class TellerRenderer :public App {
public:
	void setup();
	void CreateNewWindow();
	void Render();
};