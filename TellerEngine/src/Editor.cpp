#include"Editor.h"

void Teller::TopLevelMenu::Tick()
{
	ImGui::Begin("");
}

void Teller::EpisodeEditor::Tick()
{
	ModuleCore::Tick();
	/*
	ImGui::Begin~----;

	ImGui::End~----;
	���̊Ԃɏ����������B
	*/


	if (ImGui::Begin("Episode Editor")) {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::MenuItem("Close")) {

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		// 1. �G�f�B�^����
		{
			ImGui::BeginChild("left pane", ImVec2(300, 0), true);

			ImGui::EndChild();
		}

		// 2. �G�f�B�^�E��
		ImGui::End();
	}
}

void Teller::AssetViewer::Tick()
{
}
