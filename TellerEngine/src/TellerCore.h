#pragma once
#include"Core.h"
#include"Asset.h"
#include"Episode.h"

namespace Teller {
	class TellerCore :public ModuleCore {
	private:
		std::unique_ptr<ContentManager<Asset>> SpriteManager;
		std::unique_ptr<ContentManager<Episode>> EpisodeManager;
	public:
		TellerCore() :ModuleCore() {};
		~TellerCore() {};
	};

}