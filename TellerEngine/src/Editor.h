#pragma once
#include"Core.h"
#include"TellerCore.h"
#include"ContentManager.h"
#include"Episode.h"

namespace Teller {
	class TellerCore;

	enum class EDITOR_TYPE
	{
		NODE_EDITOR,
		EPISODE_EDITOR
	};

	class Editor :public std::enable_shared_from_this<Editor> {
	private:
	protected:
	public:
		std::weak_ptr<TellerCore> parent;

		Editor() = default;
		virtual ~Editor() = default;
		virtual void Tick();
		virtual void Update();

		//�R�s�[�֎~
		Editor(const Editor&) = delete;
		Editor& operator=(const Editor&) = delete;
		//���[�u����
		Editor& operator=(Editor&&) = default;
	};

	class TopLevelMenu :public Editor {
	private:
	public:
		TopLevelMenu() : Editor() {};
		void Tick() override;
	};

	class EpisodeEditor :public Editor {
	private:
		//�ǂݍ��܂ꂽ����CSV�t�@�C�����X�g
		std::vector<std::string> loadedCsvFiles;
		//�R���e���c�}�l�[�W���[�ւ̃|�C���^�B
		std::weak_ptr<ContentsManager<CSVLoader>> ptr_csvContentManger;

	public:
		EpisodeEditor() :Editor() {};
		~EpisodeEditor()=default;
		void Tick() override;

		void UpdateHandler();
	};

	class AssetViewer :public Editor {
	private:
	public:
		AssetViewer() :Editor() {};
		void Tick() override;
	};
}