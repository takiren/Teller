#include "Core.h"

using namespace Teller;

ModuleCore::ModuleCore()
{
}

ModuleCore::~ModuleCore()
{
}
void ModuleCore::AddChildModule(std::shared_ptr<ModuleCore>&& sub_module)
{
	children.push_back(sub_module);
	sub_module->parent = this->shared_from_this();
}

void Teller::ModuleCore::Tick()
{
	if (bUpdate) {
		for (auto& e : children) {
			e.get()->Tick();
			e.get()->Update();
		}
		return;
	}
	else
	{
		for (auto& e : children) {
			e.get()->Tick();
		}
		return;
	}
}

void Teller::ModuleCore::Update()
{
}
