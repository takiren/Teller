#pragma once
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<queue>

#include"ModuleCore.h"
#include"Scene.h"
#include"Episode.h"
#include<../json/include/nlohmann/json.hpp>

using json = nlohmann::json;

namespace Teller {
	class Sequence {
	private:
		json jsfile;
	public:
		Sequence(std::string path);
	};

	class GameModule :public ModuleCore {
		// �V�[���̓L���[�ŊǗ����邱�Ƃɂ����B
		std::queue<std::shared_ptr<SceneModule>> mScenes_;
	public:
		GameModule() :ModuleCore() {};
		~GameModule() {};
		void PushScene(std::shared_ptr<SceneModule> _scene);
		void Tick(float& deltaTime)override;
		void SetEnable(bool enable);
		bool IsEnabled() const { return bEnabled; };
		bool IsUpdate() const { return bUpdate; };
	};

}