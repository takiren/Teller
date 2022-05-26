#include "Game.h"
#include<iostream>
#include<fstream>
#include <string>
#include <cstdlib>
#include<istream>
#include<ostream>

using namespace Teller;

void Teller::GameModule::SetEnable(bool enable)
{
	bEnabled = enable;
}

void Teller::GameStory::Tick()
{
	GameModule::Tick();
}

void Teller::GameStory::Update()
{
	ImGui::Text("");
}

Teller::Sequence::Sequence(std::string path)
{
	std::ifstream reading(path);
	reading >> jsfile;
}
