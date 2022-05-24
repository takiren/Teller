#pragma once
#include "Core.h"

namespace Teller {
	class GameModule :public ModuleCore {
	public:
		GameModule();
		void SetEnable(bool enable);
		bool IsEnabled() const { return bEnabled; };
		bool IsUpdate() const { return bUpdate; };
	};
}