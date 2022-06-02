#pragma once
#include<memory>
#include<vector>
#include<map>
#include<time.h>

#include <io.h>
#include <Fcntl.h>
#include<Windows.h>

#include<cinder/Cinder.h>
#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>

#include"Core.h"
#include"ModuleCore.h"
#include"TellerEvent.h"
#include"ContentManager.h"
#include"Editor.h"
#include"Episode.h"
#include"ThreadPool.h"
#include"cinder/CinderImGui.h"
#include"Animation.h"
#include"japaneseGryph.h"

#ifdef _DEBUG
# define DEBUG_PUTS(str) puts(str)
# define DEBUG_PRINTF(fmt, ...)  printf(fmt, __VA_ARGS__);                   
#else
# define DEBUG_PUTS(str)
# define DEBUG_PRINTF(fmt, ...)
#endif

namespace Teller {
	//前方宣言
	class Editor;
	class ModuleCore;
	using CSVManager = ContentsManager<CSVLoader>;
	using SpriteManager = ContentsManager<Sprite>;
	using EpisodeManager = ContentsManager<Episode>;

	class TellerCore :
		public std::enable_shared_from_this<TellerCore>
	{
	private:
		float timeOld;
		float timeCurrent;
		float deltaTime_;
		//*Managerのポインタ保持
		std::shared_ptr<SpriteManager> spriteContentManager;
		std::shared_ptr<EpisodeManager> episodeContentManager;
		std::shared_ptr<CSVManager> CSVContentManager;
		std::vector<std::shared_ptr<ModuleCore>> modules;
		std::vector<std::shared_ptr<Editor>> editors;
		std::unique_ptr < TMessanger<int, float>> DeltaTimeMessanger;
		std::unique_ptr < TMessanger < std::string, std::vector<std::string>>> EpisodeMessanger;
		std::vector<std::shared_ptr<AnimationSequencer>> animSequencer_;

		//std::map<CALL_BACK_EVENT, std::map<int, std::function<void()>&>> callBackByEventMap;
		void CoreInitialize();
		void UpdateDeltaTime();
		int hConsole;

	public:
		TellerCore() :
			spriteContentManager(std::make_shared<SpriteManager>()),
			episodeContentManager(std::make_shared<EpisodeManager>()),
			CSVContentManager(std::make_shared<CSVManager>()),
			timeOld(0.0f),
			timeCurrent(0.01),
			deltaTime_(0.01),
			DeltaTimeMessanger(std::make_unique<TMessanger<int, float>>()),
			hConsole(0)
		{
			CoreInitialize();
		};

		TellerCore(
			std::shared_ptr<SpriteManager> _spriteManager,
			std::shared_ptr<EpisodeManager> _episodeManager,
			std::shared_ptr<CSVManager> _csvManager) :
			spriteContentManager(_spriteManager),
			episodeContentManager(_episodeManager),
			CSVContentManager(_csvManager),
			timeOld(0.0f),
			timeCurrent(0.01),
			deltaTime_(0.01),
			DeltaTimeMessanger(std::make_unique<TMessanger<int, float>>()),
			hConsole(0)
		{
			CoreInitialize();
		};


		~TellerCore() = default;
		//必ずweak_ptrで受け取ること。
		const std::shared_ptr<CSVManager>& GetCSVContentsManager() const { return CSVContentManager; };
		const std::shared_ptr<SpriteManager>& GetSpriteContentManager() const { return spriteContentManager; };
		const std::shared_ptr<EpisodeManager>& GetEpisodeContentManager() const { return episodeContentManager; };

		int AddEditor(std::shared_ptr<Editor> editor);
		int AddModule(std::shared_ptr<ModuleCore>&& sub_module);

		float GetDeltaTime()const { return deltaTime_; };

		void Tick();
		//推奨
		void AttachEvent(TEVENT_MESSAGE _event, std::shared_ptr<Editor> editor);

		void AttachDeltaTimeMessanger(int key, std::function<void(float)> callback_);

		void AddAnimSequencer(std::shared_ptr<AnimationSequencer> _animSequencer);

		void LoadCSV(std::string path);
		void LoadSprite(std::string path);

		void AddEpisode(uint64_t _key, std::unique_ptr<Episode> _episode);
	};
}