#pragma once
#include<memory>
#include<vector>
#include<map>
#include<time.h>
#include<stack>
#include<Windows.h>

#include <io.h>
#include <Fcntl.h>

#include<cinder/Cinder.h>
#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>

#include"Core.h"
#include"Game.h"
#include"TellerEvent.h"
#include"ContentManager.h"
#include"Editor.h"
#include"Episode.h"
#include"ThreadPool.h"
#include"cinder/CinderImGui.h"
#include"Animation.h"
#include"Scene.h"

#include"japaneseGryph.h"

#ifdef _DEBUG
# define DEBUG_PUTS(str) puts(str)
# define DEBUG_PRINTF(fmt, ...)  printf(fmt, __VA_ARGS__);                   
#else
# define DEBUG_PUTS(str)
# define DEBUG_PRINTF(fmt, ...)
#endif

namespace teller {
	//前方宣言
	class Editor;
	class GameModule;

	using CSVManager = ContentsManager<CSVLoader>;
	using SpriteManager = ContentsManager<Sprite>;
	using EpisodeManager = ContentsManager<Episode>;

	class TellerCore final :
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
		std::vector<std::shared_ptr<GameModule>> modules;
		std::vector<std::shared_ptr<Editor>> editors;

		std::stack<std::unique_ptr<GameModule>> gameModuleStack;

		//メッセンジャーのポインタ
		//メッセージベース駆動
		std::unique_ptr < TMessanger<int, float>> DeltaTimeMessangerRef;
		std::unique_ptr < TMessanger < std::string, std::vector<std::string>>> EpisodeMessangerRef;
		std::unique_ptr< TMessanger<int, fs::path>> pathMessangerRef;

		//シーケンサーはいずれ削除する。
		std::vector<std::shared_ptr<AnimationSequencer>> animSequencer_;

		std::unordered_map<fs::path, std::unique_ptr<Editor>> editorsRef;

		//std::map<CALL_BACK_EVENT, std::map<int, std::function<void()>&>> callBackByEventMap;
		//初期化処理
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
			pathMessangerRef(std::make_unique<TMessanger<int, fs::path>>()),
			timeOld(0.0f),
			timeCurrent(0.01),
			deltaTime_(0.01),
			DeltaTimeMessangerRef(std::make_unique<TMessanger<int, float>>()),
			hConsole(0)
		{
			CoreInitialize();
		};


		~TellerCore() = default;

		TellerCore(const TellerCore&) = delete;
		TellerCore& operator=(const TellerCore&) = delete;

		TellerCore& operator=(TellerCore&&) = default;


		//毎チック行う処理。
		void Tick();

		//モジュール追加

		void AddGame(std::unique_ptr<GameModule>&& gamemodule) {
			gameModuleStack.push(std::move(gamemodule));
		}

		//エディターの追加。ファイルを編集するエディターの場合は対応する拡張子を引数に与える。
		void AddEditor(fs::path _extension, std::unique_ptr<Editor> editor);
		void AddEditor(std::unique_ptr<Editor> editor);

		//ファイルをロード
		void LoadFileToEditor(fs::path _file);

		float GetDeltaTime()const { return deltaTime_; };

		//TODO:Delete
		//depricated
		void AttachEvent(TEVENT_MESSAGE _event, std::shared_ptr<Editor> editor);

		//これもいらん
		//TODO:Delete
		void AttachDeltaTimeMessanger(int key, std::function<void(float)> callback_);

		//TODO:Delete
		void AddAnimSequencer(std::shared_ptr<AnimationSequencer> _animSequencer);

		//TODO:Delete
		void AddEpisode(uint64_t _key, std::unique_ptr<Episode> _episode);

		//Depricated:Use GetActiveGameModule()
		//CAUTION:It returns "Raw pointer". Be careful not to delete.
		GameModule* GetGameModuleRef() { return gameModuleStack.top().get(); }

		//CAUTION:It returns "Raw pointer". Be careful not to delete.
		GameModule* GetActiveGameModule() { return gameModuleStack.top().get(); }

		std::shared_ptr<SceneModule> GetActiveScene() { return gameModuleStack.top()->GetActiveScene(); }
	};
}