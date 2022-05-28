#include"Editor.h"

void Teller::TopLevelMenu::Tick()
{
	ImGui::Begin("");
}

void Teller::EpisodeEditor::Tick()
{
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

		static int selectedFile = 0;

		std::string selectedFileKey{ "" };

		// 1. �G�f�B�^����
		{
			ImGui::BeginChild("left pane", ImVec2(300, 0), true);
			ImGui::Text("Loaded files.");
			// ���[�h���ꂽ�t�@�C���������ɕ\���B
			{
				selectedFileKey = "";
				int i = 0;
				for (auto& e : loadedCsvFiles) {
					if (ImGui::Selectable(e.c_str(), selectedFile == i)) {
						selectedFile = i;
						selectedFileKey = loadedCsvFiles.at(i);
					}
					i++;
				}
			}
			ImGui::EndChild();
		}

		static int curr=0;
		// 2. �G�f�B�^�E��

		{
			ImGui::BeginGroup();
			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
			ImGui::Text("Selected File: %d", curr);
			ImGui::Separator();
			// CSV�t�@�C���̒��g��\���B
			static int currentLine = 0;
			{

			}
			if (ImGui::Button("Revert")) {}
			ImGui::SameLine();
			if (ImGui::Button("Save")) {}
			ImGui::EndGroup();
		}

		ImGui::End();
	}
}

void Teller::AssetViewer::Tick()
{
}

void Teller::Editor::Tick()
{
}

void Teller::Editor::Update()
{
}
