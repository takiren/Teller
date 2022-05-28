#pragma once
#include<memory>
#include<vector>
#include<map>

#include<Core.h>
#include"ModuleCore.h"
#include"TellerEvent.h"
#include"ContentManager.h"
#include"Editor.h"
#include"Episode.h"
#include"ThreadPool.h"

namespace Teller {
	class Editor;
	class ModuleCore;
	using CSVManager = ContentsManager<CSVLoader>;
	using SpriteManager = ContentsManager<Sprite>;
	using EpisodeManager = ContentsManager<Episode>;

	class TellerCore :public std::enable_shared_from_this<TellerCore> {
	private:
		//*Managerのポインタ保持
		std::shared_ptr<SpriteManager> spriteContentManager;
		std::shared_ptr<EpisodeManager> episodeContentManager;
		std::shared_ptr<CSVManager> CSVContentManager;

		std::vector<std::shared_ptr<ModuleCore>> modules;
		std::vector<std::shared_ptr<Editor>> editors;

		

		//std::map<CALL_BACK_EVENT, std::map<int, std::function<void()>&>> callBackByEventMap;
		void CoreInitialize();
	public:
		TellerCore() :
			spriteContentManager(std::make_shared<SpriteManager>()),
			episodeContentManager(std::make_shared<EpisodeManager>()),
			CSVContentManager(std::make_shared<CSVManager>())
		{};

		TellerCore(
			std::shared_ptr<SpriteManager> CMSprite,
			std::shared_ptr<EpisodeManager> CMEpisode,
			std::shared_ptr<CSVManager> CMCSV)
			:
			spriteContentManager(CMSprite),
			episodeContentManager(CMEpisode),
			CSVContentManager(CMCSV)
		{};

		~TellerCore() = default;
		//必ずweak_ptrで受け取ること。
		std::shared_ptr<CSVManager> GetCSVContentsManager() const { return CSVContentManager; };
		std::shared_ptr<SpriteManager> GetSpriteContentManager() const { return spriteContentManager; };
		std::shared_ptr<EpisodeManager> GetEpisodeContentManager() const { return episodeContentManager; };

		int AddEditor(std::shared_ptr<Editor> editor);
		int AddModule(std::shared_ptr<ModuleCore> sub_module);
		void Tick(float& deltaTime);
		//推奨
		void AttachEvent(CALL_BACK_EVENT _event, std::shared_ptr<Editor> editor);
	};
}