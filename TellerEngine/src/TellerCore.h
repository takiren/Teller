#pragma once
#include"Core.h"
#include"Asset.h"
#include"Episode.h"

namespace Teller {
	class TellerCore :public ModuleCore {
	private:
		//*Manager�̃|�C���^�ێ�
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
		//�K��weak_ptr�Ŏ󂯎�邱�ƁB
		std::shared_ptr<ContentManager<CSVLoader>> GetCSVContents() const { return CSVContentManager; };
		std::shared_ptr<ContentManager<Sprite>> GetSpriteContentManager() const { return spriteContentManager; };
		std::shared_ptr<ContentManager<Episode>> GetEpisodeContentManager() const { return episodeContentManager; };
	};
}