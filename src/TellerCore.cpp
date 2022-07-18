#include"TellerCore.h"

namespace teller {
	void TellerCore::Tick()
	{
		auto now = clock();
		DeltaTimeMessangerRef->SendTMessage(deltaTime_);
		// 1. ���W���[����Tick()�Ăяo���B
		{
			gameModuleStack.top()->Tick(deltaTime_);
		}

		for (auto& e : animSequencer_) {
			e->Update();
		}

		auto old = clock();
		deltaTime_ = old - now;
	}

	void TellerCore::EditorTick()
	{
		for (auto& e : editorsRef)
			e.second->Tick();
	}

	void TellerCore::AttachEvent(TEVENT_MESSAGE _event, std::shared_ptr<Editor> editor)
	{
	}

	void TellerCore::AttachDeltaTimeMessanger(int key, std::function<void(float)> callback_)
	{
		DeltaTimeMessangerRef->AttachFunction(key, callback_);
	}

	void TellerCore::EditorDraw()
	{
		for (auto& e : editorsRef)
			e.second->Draw();
	}

	void TellerCore::EditorUpdate()
	{
	}

	void TellerCore::AddAnimSequencer(std::shared_ptr<AnimationSequencer> _animSequencer)
	{
		animSequencer_.push_back(_animSequencer);
	}

	void TellerCore::AddEpisode(uint64_t _key, std::unique_ptr<Episode> _episode)
	{
		episodeContentManagerRef->AddContent(_key, std::move(_episode));
	}

	void TellerCore::LoadFileToEditor(fs::path _file)
	{
		editorsRef[_file.extension()]->LoadFile(_file);
	}

	void TellerCore::AddEditor(fs::path _extension, std::unique_ptr<Editor> editor)
	{
		editor->parent = this->shared_from_this();
		editorsRef[_extension] = std::move(editor);
	}
	void TellerCore::AddEditor(std::unique_ptr<Editor> editor)
	{
		editor->parent = this->shared_from_this();
		editorsRef[editor->name_] = std::move(editor);
	}
	void TellerCore::CoreInitialize()
	{
		DeltaTimeMessangerRef = std::make_unique<TMessanger<int, float>>();
	}

	void TellerCore::CreateThreadPool()
	{

	}

	void TellerCore::UpdateDeltaTime()
	{
		deltaTime_ = timeCurrent - timeOld;
	}
}