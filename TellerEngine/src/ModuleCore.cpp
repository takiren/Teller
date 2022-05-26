#include"ModuleCore.h"

using namespace Teller;

void Teller::ModuleCore::AddChildModule(std::shared_ptr<ModuleCore> sub_module)
{
	children.push_back(sub_module);
	sub_module->parent = shared_from_this();
}


void Teller::ModuleCore::Update()
{

}

void Teller::ModuleCore::Tick() {
	if (!bEnabled) return;
	count_++;
	if (bUpdate) {
		for (auto& e : children) {
			e.get()->Tick();
			e.get()->Update();
		}
	}
	else
	{
		for (auto& e : children) {
			e.get()->Tick();
		}
	}
}