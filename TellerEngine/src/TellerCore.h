#pragma once
#include"Core.h"
#include"ContentManager.h"

namespace Teller {
	class Editor;
	class Sprite;
	class CSVLoader;
	template<class TYPE>
	class ContentManager;
	class Episode;

	class TellerCore{
	private:
		//*Managerのポインタ保持
		std::shared_ptr<ContentManager<Sprite>> spriteContentManager;
		std::shared_ptr<ContentManager<Episode>> episodeContentManager;
		std::shared_ptr<ContentManager<CSVLoader>> CSVContentManager;

		std::vector<std::shared_ptr<ModuleCore>> modules;
		std::vector<std::shared_ptr<Editor>> editors;
	public:
		TellerCore():
			spriteContentManager(std::make_shared<ContentManager<Sprite>>()),
			episodeContentManager(std::make_shared<ContentManager<Episode>>()),
			CSVContentManager(std::make_shared<ContentManager<CSVLoader>>())
		{};
		~TellerCore()=default;
		//必ずweak_ptrで受け取ること。
		std::shared_ptr<ContentManager<CSVLoader>> GetCSVContents() const { return CSVContentManager; };
		std::shared_ptr<ContentManager<Sprite>> GetSpriteContentManager() const { return spriteContentManager; };
		std::shared_ptr<ContentManager<Episode>> GetEpisodeContentManager() const { return episodeContentManager; };
		
		void Tick();
	};
}