#pragma once
#include<stack>
#include<algorithm>
#include<memory>
#include<deque>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>

#include"TellerCore.h"
#include"Scene.h"
#include"Episode.h"

#include<../json/include/nlohmann/json.hpp>

using json = nlohmann::json;

namespace teller {
	class TellerCore;

	class Sequence {
	private:
		json jsfile;
	public:
		Sequence(std::string path);
	};

	class GameModule {
		// シーンはキューで管理することにした。
		std::weak_ptr<TellerCore> owner;

		std::stack<std::shared_ptr<SceneModule>> mScenes_;

		bool bEnabled = true;
	public:
		GameModule()=default;
		virtual ~GameModule() = default;
		void PushScene(std::shared_ptr<SceneModule> _scene);
		void Tick(float& deltaTime);

		void SetOwner(std::shared_ptr<TellerCore> _tellercore) { owner = _tellercore; }
	};

}