
#ifndef _SRC_TELLERCORE_H_
#define _SRC_TELLERCORE_H_

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

namespace teller {
	//�O���錾
	class Editor;
	class GameModule;

	using CSVManager = ContentsManager<CSVLoader>;
	using SpriteManager = ContentsManager<Sprite>;
	using EpisodeManager = ContentsManager<Episode>;

	class TellerCore final :
		public std::enable_shared_from_this<TellerCore>
	{
	private:
		//dt�v�Z�̂��߂̕ϐ��B
		float timeOld;
		float timeCurrent;
		float deltaTime_;

		//ContentsManager�̃|�C���^�ێ�
		std::shared_ptr<SpriteManager> spriteContentManagerRef;
		std::shared_ptr<EpisodeManager> episodeContentManagerRef;
		std::shared_ptr<CSVManager> CSVContentManagerRef;

		//���W���[���ƃG�f�B�^�[�̂���B
		std::vector<std::shared_ptr<GameModule>> modules;
		std::vector<std::shared_ptr<Editor>> editors;

		std::stack<std::unique_ptr<GameModule>> gameModuleStack;

		//���b�Z���W���[�̃|�C���^
		//���b�Z�[�W�x�[�X�쓮
		std::unique_ptr < TMessanger<int, float>> DeltaTimeMessangerRef;
		std::unique_ptr < TMessanger < std::string, std::vector<std::string>>> EpisodeMessangerRef;
		std::unique_ptr< TMessanger<int, fs::path>> pathMessangerRef;

		//�V�[�P���T�[�͂�����폜����B
		std::vector<std::shared_ptr<AnimationSequencer>> animSequencer_;

		std::unordered_map<fs::path, std::unique_ptr<Editor>> editorsRef;

		//std::map<CALL_BACK_EVENT, std::map<int, std::function<void()>&>> callBackByEventMap;
		//����������
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


		//���`�b�N�s�������B
		void Tick();

		//���W���[���ǉ�

		void AddGame(std::unique_ptr<GameModule>&& gamemodule) {
			gameModuleStack.push(std::move(gamemodule));
		}

		//�G�f�B�^�[�̒ǉ��B�t�@�C����ҏW����G�f�B�^�[�̏ꍇ�͑Ή�����g���q�������ɗ^����B
		void AddEditor(fs::path _extension, std::unique_ptr<Editor> editor);
		void AddEditor(std::unique_ptr<Editor> editor);

		//�t�@�C�������[�h
		void LoadFileToEditor(fs::path _file);

		float GetDeltaTime()const { return deltaTime_; };

		//TODO:Delete
		//depricated
		void AttachEvent(TEVENT_MESSAGE _event, std::shared_ptr<Editor> editor);

		//����������
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

#endif // !_SRC_TELLERCORE_H_

