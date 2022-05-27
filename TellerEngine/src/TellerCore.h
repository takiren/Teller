#pragma once
#include"Core.h"
#include"ModuleCore.h"
#include"TellerEvent.h"
#include"ContentManager.h"
#include"Editor.h"
#include"Episode.h"

namespace Teller {
	class Editor;
	class ModuleCore;

	

	class TellerCore :public std::enable_shared_from_this<TellerCore> {
	private:
		//*Managerのポインタ保持
		std::shared_ptr<ContentsManager<Sprite>> spriteContentManager;
		std::shared_ptr<ContentsManager<Episode>> episodeContentManager;
		std::shared_ptr<ContentsManager<CSVLoader>> CSVContentManager;

		std::vector<std::shared_ptr<ModuleCore>> modules;
		std::vector<std::shared_ptr<Editor>> editors;

		//std::map<CALL_BACK_EVENT, std::map<int, std::function<void()>&>> callBackByEventMap;

	public:
		TellerCore() :
			spriteContentManager(std::make_shared<ContentsManager<Sprite>>()),
			episodeContentManager(std::make_shared<ContentsManager<Episode>>()),
			CSVContentManager(std::make_shared<ContentsManager<CSVLoader>>())
		{};

		TellerCore(
			std::shared_ptr<ContentsManager<Sprite>> CMSprite,
			std::shared_ptr<ContentsManager<Episode>> CMEpisode,
			std::shared_ptr<ContentsManager<CSVLoader>> CMCSV)
			:
			spriteContentManager(CMSprite),
			episodeContentManager(CMEpisode),
			CSVContentManager(CMCSV)
		{};

		~TellerCore() = default;
		//必ずweak_ptrで受け取ること。
		std::shared_ptr<ContentsManager<CSVLoader>> GetCSVContentsManager() const { return CSVContentManager; };
		std::shared_ptr<ContentsManager<Sprite>> GetSpriteContentManager() const { return spriteContentManager; };
		std::shared_ptr<ContentsManager<Episode>> GetEpisodeContentManager() const { return episodeContentManager; };

		int AddEditor(std::shared_ptr<Editor> editor);
		int AddModule(std::shared_ptr<ModuleCore> sub_module);
		void Tick();
		//推奨
		void AttachEvent(CALL_BACK_EVENT _event, std::shared_ptr<Editor> editor);
	};
}