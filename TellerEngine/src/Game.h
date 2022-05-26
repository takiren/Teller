#pragma once
#include "Core.h"
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
		std::vector<Episode> episodes;

	public:
		GameModule() :ModuleCore() {};
		~GameModule() {};
		void SetEnable(bool enable);
		bool IsEnabled() const { return bEnabled; };
		bool IsUpdate() const { return bUpdate; };
	};

	class GameStory :public GameModule {
	private:
		std::vector<Episode> episodes;
	public:
		GameStory(Episode episode) :
			GameModule()
		{
			episodes.push_back(episode);
		};

		~GameStory() = default;
		void Tick() override;
		void Update() override;
	};
}