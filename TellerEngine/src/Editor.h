#pragma once
#include"Core.h"


namespace Teller {
	enum class EDITOR_TYPE
	{
		NODE_EDITOR,
		EPISODE_EDITOR
	};

	class Editor :public ModuleCore{
	private:
		
	public:

	};

	class TopLevelMenu :public ModuleCore{
	private:
	public:
		void Tick() override;

	};

	class EpisodeEditor :public ModuleCore {
	private:
	public:
		void Tick() override;
	};


	class AssetViewer :public ModuleCore {
	private:
	public:
		void Tick() override;
	};
}