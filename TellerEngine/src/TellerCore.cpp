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
	void TellerCore::AddModule(std::shared_ptr<ModuleCore> sub_module)
	{
		modules.push_back(sub_module);
		sub_module->ptrTellerCore = this->shared_from_this();
	}
	void TellerCore::AddEditor(std::shared_ptr<Editor> editor)
	{
		editors.push_back(editor);
		editor->parent = this->shared_from_this();
	}
}