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
	この間に処理を書く。
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

		// 1. エディタ左側
		{
			ImGui::BeginChild("left pane", ImVec2(300, 0), true);
			ImGui::Text("Loaded files.");
			// ロードされたファイルを左側に表示。
			{
				selectedFileKey = "";
				int i=0;
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

		static int curr;
		// 2. エディタ右側

		{
			ImGui::BeginGroup();
			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
			ImGui::Text("Selected File: %d", curr);
			ImGui::Separator();
			// CSVファイルを表示。
			static int currentLine = 0;
			{
				std::weak_ptr<CSVLoader> data = ptr_csvContentManger.lock()->GetContent(loadedCsvFiles.at(selectedFile));

				std::vector<std::string> d;
				for (size_t i = 0; i < data.lock().get()->GetCSVData().size(); i++)
				{
					auto& t = data.lock().get()->GetCSVData().at(i);
					std::string s{ "" };
					for (auto& e : t) {
						s += e;
					}
					d.push_back(s);
					if (ImGui::Selectable(s.c_str(), curr == i)) {
						curr = i;
					}
				}

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

void Teller::EpisodeEditor::UpdateKeys()
{
	loadedCsvFiles = ptr_csvContentManger.lock()->GetKeys();
}
