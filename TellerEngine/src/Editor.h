#pragma once
#include"Core.h"
#include"ContentManager.h"
#include"Episode.h"
#include"TellerCore.h"

namespace Teller {

	enum class EDITOR_TYPE
	{
		NODE_EDITOR,
		EPISODE_EDITOR
	};

	class Editor :public ModuleCore{
	private:
	protected:
		// ëΩï™parentÇ…ê›íËÇ∑ÇÈä÷êîÇ‡íäè€âªÇµÇ»Ç¢Ç∆Ç§Ç‹Ç≠ìÆÇ©Ç»Ç¢
		std::weak_ptr<TellerCore> parent;
	public:
	};

	class TopLevelMenu :public Editor{
	private:
	public:
		TopLevelMenu() : Editor(){};
		void Tick() override;
	};

	class EpisodeEditor :public Editor {
	private:
		std::weak_ptr<ContentManager<CSVLoader>> wptr_csvContentManger;
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