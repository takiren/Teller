#pragma once
#include"Core.h"

#include"Asset.h"
#include"ContentManager.h"
#include"Editor.h"
#include"Episode.h"
#include"ModuleCore.h"


namespace Teller {
	class Editor;
	class ModuleCore;

	class TellerCore :public std::enable_shared_from_this<TellerCore> {
	private:
		//*Managerのポインタ保持
		std::shared_ptr<ContentManager<Sprite>> spriteContentManager;
		std::shared_ptr<ContentManager<Episode>> episodeContentManager;
		std::shared_ptr<ContentManager<CSVLoader>> CSVContentManager;

		std::vector<std::shared_ptr<ModuleCore>> modules;
		std::vector<std::shared_ptr<Editor>> editors;
	public:
		TellerCore() :
			spriteContentManager(std::make_shared<ContentManager<Sprite>>()),
			episodeContentManager(std::make_shared<ContentManager<Episode>>()),
			CSVContentManager(std::make_shared<ContentManager<CSVLoader>>())
		{};

		TellerCore(
			std::shared_ptr<ContentManager<Sprite>> CMSprite,
			std::shared_ptr<ContentManager<Episode>> CMEpisode,
			std::shared_ptr<ContentManager<CSVLoader>> CMCSV) 
			:
			spriteContentManager(CMSprite),
			episodeContentManager(CMEpisode),
			CSVContentManager(CMCSV) 
		{};

		~TellerCore() = default;
		//必ずweak_ptrで受け取ること。
		std::shared_ptr<ContentManager<CSVLoader>> GetCSVContentsManager() const { return CSVContentManager; };
		std::shared_ptr<ContentManager<Sprite>> GetSpriteContentManager() const { return spriteContentManager; };
		std::shared_ptr<ContentManager<Episode>> GetEpisodeContentManager() const { return episodeContentManager; };

		void AddEditor(std::shared_ptr<Editor> editor);
		void AddModule(std::shared_ptr<ModuleCore> sub_module);
		void Tick();
	};
}