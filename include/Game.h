#pragma once
#include<stack>
#include<algorithm>
#include<memory>
#include<deque>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>

#include"Scene.h"
#include"Episode.h"

#include<nlohmann/json.hpp>

using json = nlohmann::json;

namespace teller {

	class Sequence {
	private:
		json jsfile;
	public:
		Sequence(std::string path);
	};

	class GameModule {
		// シーンはキューで管理することにした。
		//std::weak_ptr<TellerCore> owner;
		std::stack<std::shared_ptr<SceneModule>> mScenes_;

		bool bEnabled = true;

		bool bUpdate;
	public:
		GameModule() :
			bUpdate(false)
		{};
		virtual ~GameModule() = default;
		void PushScene(std::shared_ptr<SceneModule> _scene);
		void Tick(float& deltaTime);

		//void SetOwner(std::shared_ptr<TellerCore> _tellercore) { owner = _tellercore; }

		GameModule(const GameModule&) = delete;
		GameModule& operator=(const GameModule&) = delete;

		GameModule& operator=(GameModule&&) = default;

		std::shared_ptr<SceneModule> GetActiveScene() { return mScenes_.top(); }

	};

}