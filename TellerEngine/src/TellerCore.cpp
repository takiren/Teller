#include"TellerCore.h"

namespace Teller {
	void TellerCore::Tick()
	{
		auto now = clock();
		DeltaTimeMessanger->SendMessage(deltaTime_);
		// 1. モジュールのTick()呼び出し。
		{
			for (auto& e : modules) {
				e->Tick(deltaTime_);
			}
		}

		// 2. エディターのTick()呼び出し。
		{
			for (auto& e : editors) {
				e->Tick();
			}
		}

		for (auto& e : animSequencer_) {
			e->Update();
		}

		auto old = clock();
		deltaTime_ = old - now;
		ImGui::Text("deltatime %f", deltaTime_);
	}

	void TellerCore::AttachEvent(TEVENT_MESSAGE _event, std::shared_ptr<Editor> editor)
	{
		//callBackByEventMap.at(_event)[callBackByEventMap.at(_event).size()] = 0;

	}

	void TellerCore::AttachDeltaTimeMessanger(int key, std::function<void(float)> callback_)
	{
		DeltaTimeMessanger->AttachFunction(key, callback_);
	}

	void TellerCore::AddAnimSequencer(std::shared_ptr<AnimationSequencer> _animSequencer)
	{
		animSequencer_.push_back(_animSequencer);
	}

	int TellerCore::AddModule(std::shared_ptr<ModuleCore>&& sub_module)
	{
		modules.push_back(sub_module);
		sub_module->ptrTellerCore = this->shared_from_this();
		return 0;
	}
	void TellerCore::CoreInitialize()
	{
		DeltaTimeMessanger = std::make_unique<TMessanger<int, float>>();

	}
	void TellerCore::UpdateDeltaTime()
	{
		deltaTime_ = timeCurrent - timeOld;
	}
	int TellerCore::AddEditor(std::shared_ptr<Editor>&& editor)
	{
		editors.push_back(editor);
		editor->parent = this->shared_from_this();
		return 0;
	}
}