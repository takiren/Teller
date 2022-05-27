#include"TellerCore.h"

namespace Teller {
	void TellerCore::Tick()
	{
		// 1. ���W���[����Tick()�Ăяo���B
		{
			for (auto& e : modules) {
				e.get()->Tick();
			}
		}

		// 2. �G�f�B�^�[��Tick()�Ăяo���B
		{

		}
	}

	void TellerCore::AttachEvent(CALL_BACK_EVENT _event, std::shared_ptr<Editor> editor)
	{
		//callBackByEventMap.at(_event)[callBackByEventMap.at(_event).size()] = 0;

	}

	int TellerCore::AddModule(std::shared_ptr<ModuleCore> sub_module)
	{
		modules.push_back(sub_module);
		sub_module->ptrTellerCore = this->shared_from_this();
		return 0;
	}
	int TellerCore::AddEditor(std::shared_ptr<Editor> editor)
	{
		editors.push_back(editor);
		editor->parent = this->shared_from_this();
		return 0;
	}
}