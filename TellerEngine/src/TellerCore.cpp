#include"TellerCore.h"

namespace Teller {
	void TellerCore::Tick(float& deltaTime)
	{
		DeltaTimeMessanger.SendMessage(deltaTime);
		// 1. モジュールのTick()呼び出し。
		{
			for (auto& e : modules) {
				e->Tick(deltaTime);
			}
		}

		// 2. エディターのTick()呼び出し。
		{
			for (auto& e : editors) {
				e->Tick();
			}
		}
	}

	void TellerCore::AttachEvent(TEVENT_MESSAGE _event, std::shared_ptr<Editor> editor)
	{
		//callBackByEventMap.at(_event)[callBackByEventMap.at(_event).size()] = 0;

	}

	void TellerCore::AttachDeltaTimeMessanger(int key,std::function<void(float)> callback)
	{
		DeltaTimeMessanger.AttachFunction(key, callback);
	}

	int TellerCore::AddModule(std::shared_ptr<ModuleCore> sub_module)
	{
		modules.push_back(sub_module);
		sub_module->ptrTellerCore = this->shared_from_this();
		return 0;
	}
	void TellerCore::CoreInitialize()
	{

	}
	void TellerCore::UpdateDeltaTime()
	{
		deltaTime_ = timeCurrent - timeOld;
	}
	int TellerCore::AddEditor(std::shared_ptr<Editor> editor)
	{
		editors.push_back(editor);
		editor->parent = this->shared_from_this();
		return 0;
	}
}