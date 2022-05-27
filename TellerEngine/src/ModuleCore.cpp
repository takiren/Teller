#include"ModuleCore.h"

using namespace Teller;

void Teller::ModuleCore::AddChildModule(std::shared_ptr<ModuleCore> sub_module)
{
	children.push_back(sub_module);
	sub_module->parent = shared_from_this();
	sub_module->ptrTellerCore = this->ptrTellerCore;
}

void Teller::ModuleCore::Update(){};

void Teller::ModuleCore::Tick() {
	if (!bEnabled) return;
	/*
	Tick()���ŏ�����������
	*/

	count_++;
	if (bUpdate) {
		for (auto& e : children) {
			e.get()->Tick();
		}
	}
	else
	{
		for (auto& e : children) {
			e.get()->Tick();
		}
	}
	/*
	�����܂�
	*/

	if (!bUpdate) return;
	Teller::ModuleCore::Update();
}
