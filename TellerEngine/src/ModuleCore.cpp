#include"ModuleCore.h"

using namespace Teller;

void Teller::ModuleCore::AddChildModule(std::shared_ptr<ModuleCore> sub_module)
{
	children.push_back(sub_module);
	sub_module->parent = shared_from_this();
	sub_module->ptrTellerCore = this->ptrTellerCore;
}

void Teller::ModuleCore::Update(){};

void Teller::ModuleCore::Tick(float& deltaTime) {

	//1ƒtƒŒ[ƒ€‚Ì•`‰æ‚É‚©‚©‚Á‚½ŽžŠÔ
	deltaTime_ = deltaTime;
	if (!bEnabled) return;
	/*
	Tick()“àˆ—‚±‚±‚©‚ç
	*/

	count_++;
	
	if (bUpdate) {
		for (auto& e : children) {
			e.get()->Tick(deltaTime);
		}
	}
	else
	{
		for (auto& e : children) {
			e.get()->Tick(deltaTime);
		}
	}
	/*
	‚±‚±‚Ü‚Å
	*/

	if (!bUpdate) return;
	Teller::ModuleCore::Update();
}
