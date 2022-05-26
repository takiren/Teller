#pragma once
#include"Core.h"

namespace Teller {
	class TellerCore;
	template<class TYPE>
	class ContentManager;
	class CSVLoader;
	

	enum class EDITOR_TYPE
	{
		NODE_EDITOR,
		EPISODE_EDITOR
	};

	class Editor:public std::enable_shared_from_this<Editor>{
	private:
	protected:
		std::weak_ptr<TellerCore> parent;
	public:
		Editor() = default;
		virtual void Tick();
		virtual void Update();

		Editor(const Editor&) = delete;
		Editor& operator=(const Editor&) = delete;

		Editor& operator=(Editor&&) = default;
	};

	class TopLevelMenu :public Editor{
	private:
	public:
		TopLevelMenu() : Editor(){};
		void Tick() override;
	};

	class EpisodeEditor :public Editor {
	private:

		std::weak_ptr<ContentManager<CSVLoader>> ptr_csvContentManger;
		void LinkTellerCore();
	public:
		EpisodeEditor() :
			Editor()
		{
			LinkTellerCore();
		};
		void Tick() override;
		
	};

	class AssetViewer :public Editor {
	private:
	public:
		AssetViewer() :Editor() {};
		void Tick() override;
	};
}