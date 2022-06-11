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
		//dt計算のための変数。
		float timeOld;
		float timeCurrent;
		float deltaTime_;

		//ContentsManagerのポインタ保持
		std::shared_ptr<SpriteManager> spriteContentManagerRef;
		std::shared_ptr<EpisodeManager> episodeContentManagerRef;
		std::shared_ptr<CSVManager> CSVContentManagerRef;

		//モジュールとエディターのあれ。
		std::vector<std::shared_ptr<ModuleCore>> modules;
		std::vector<std::shared_ptr<Editor>> editors;

		//メッセンジャーのポインタ
		//メッセージベース駆動
		std::unique_ptr < TMessanger<int, float>> DeltaTimeMessangerRef;
		std::unique_ptr < TMessanger < std::string, std::vector<std::string>>> EpisodeMessangerRef;
		std::unique_ptr<TMessanger<int, fs::path>> pathMessangerRef;

		//シーケンサーはいずれ削除する。
		std::vector<std::shared_ptr<AnimationSequencer>> animSequencer_;

		std::unordered_map<fs::path, std::unique_ptr<Editor>> editorsRef;

		//std::map<CALL_BACK_EVENT, std::map<int, std::function<void()>&>> callBackByEventMap;
		void CoreInitialize();
		void UpdateDeltaTime();
		int hConsole;

	public:
		TellerCore() :
			spriteContentManagerRef(std::make_shared<SpriteManager>()),
			episodeContentManagerRef(std::make_shared<EpisodeManager>()),
			CSVContentManagerRef(std::make_shared<CSVManager>()),
			timeOld(0.0f),
			timeCurrent(0.01),
			deltaTime_(0.01),
			DeltaTimeMessangerRef(std::make_unique<TMessanger<int, float>>()),
			hConsole(0)
		{
			CoreInitialize();
		};

		TellerCore(
			std::shared_ptr<SpriteManager> _spriteManager,
			std::shared_ptr<EpisodeManager> _episodeManager,
			std::shared_ptr<CSVManager> _csvManager) :
			spriteContentManagerRef(_spriteManager),
			episodeContentManagerRef(_episodeManager),
			CSVContentManagerRef(_csvManager),
			pathMessangerRef(std::make_unique<TMessanger<int,fs::path>>()),
			timeOld(0.0f),
			timeCurrent(0.01),
			deltaTime_(0.01),
			DeltaTimeMessangerRef(std::make_unique<TMessanger<int, float>>()),
			hConsole(0)
		{
			CoreInitialize();
		};


		~TellerCore() = default;

		//毎チック行う処理。
		void Tick();

		//必ずweak_ptrで受け取ること。
		const std::shared_ptr<CSVManager>& GetCSVContentsManager() const { return CSVContentManagerRef; };
		const std::shared_ptr<SpriteManager>& GetSpriteContentManager() const { return spriteContentManagerRef; };
		const std::shared_ptr<EpisodeManager>& GetEpisodeContentManager() const { return episodeContentManagerRef; };

		//エディターの追加　deprecated
		int AddEditor(std::shared_ptr<Editor> editor);

		//モジュール追加
		int AddModule(std::shared_ptr<ModuleCore>&& sub_module);

		//エディターの追加。ファイルを編集するエディターの場合は対応する拡張子を引数に与える。
		void AppendEditor(fs::path _extension, std::unique_ptr<Editor> editor);
		void AppendEditor(std::unique_ptr<Editor> editor);
			

		void LoadFileToEditor(fs::path _file);

		float GetDeltaTime()const { return deltaTime_; };
		//推奨
		void AttachEvent(TEVENT_MESSAGE _event, std::shared_ptr<Editor> editor);

		void AttachDeltaTimeMessanger(int key, std::function<void(float)> callback_);

		void AddAnimSequencer(std::shared_ptr<AnimationSequencer> _animSequencer);

		void LoadSprite(std::string path);

		void AddEpisode(uint64_t _key, std::unique_ptr<Episode> _episode);
	};
}