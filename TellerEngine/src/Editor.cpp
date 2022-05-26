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
	この間に処理を書く。
	*/


	if (ImGui::Begin("Episode Editor")) {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::MenuItem("Close")) {

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		// 1. エディタ左側
		{
			ImGui::BeginChild("left pane", ImVec2(300, 0), true);

			ImGui::EndChild();
		}

		// 2. エディタ右側
		ImGui::End();
	}
}

void Teller::AssetViewer::Tick()
{
}
