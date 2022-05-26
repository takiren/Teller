#pragma once
#include"Core.h"
#include"Asset.h"
#include"Episode.h"

namespace Teller {
	class TellerCore :public ModuleCore {
	private:
		//*Managerのポインタ保持
		std::shared_ptr<ContentManager<Sprite>> spriteContentManager;
		std::shared_ptr<ContentManager<Episode>> episodeContentManager;
		std::shared_ptr<ContentManager<CSVLoader>> CSVContentManager;
	public:
		TellerCore():
			ModuleCore(),
			spriteContentManager(std::make_shared<ContentManager<Sprite>>()),
			episodeContentManager(std::make_shared<ContentManager<Episode>>()),
			CSVContentManager(std::make_shared<ContentManager<CSVLoader>>())
		{};
		~TellerCore()=default;
		//必ずweak_ptrで受け取ること。
		std::shared_ptr<ContentManager<CSVLoader>> GetCSVContents() const { return CSVContentManager; };
		std::shared_ptr<ContentManager<Sprite>> GetSpriteContentManager() const { return spriteContentManager; };
		std::shared_ptr<ContentManager<Episode>> GetEpisodeContentManager() const { return episodeContentManager; };
	};
}