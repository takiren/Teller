#include"Editor.h"

void Teller::TopLevelMenu::Tick()
{
	ImGui::Begin("");
	ImGui::End();
}

void Teller::EpisodeEditor::Tick()
{
	/*
	ImGui::Begin~----;

	ImGui::End~----;
	この間に処理を書く。
	*/
	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Episode Editor")) {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::MenuItem("Close")) {

				ImGui::EndMenu();

				ImGui::EndMenuBar();
			}
		}

		static int selectedFile = 0;

		static std::string selectedFileKey{ "" };
		// 1. エディタ左側
		{
			ImGui::BeginChild("left pane", ImVec2(300, 0), true);
			ImGui::Text("Loaded files.");
			// ロードされたファイルを左側に表示。
			{
				selectedFileKey = "";
				int i = 0;
				auto csvvec = ptr_csvContentManger.lock()->GetKeys();
				if (csvvec.size() != 0) {
					for (auto& e : csvvec) {
						if (ImGui::Selectable(e.c_str(), selectedFile == i)) {
							selectedFile = i;
							selectedFileKey = csvvec.at(i);
						}
						i++;
					}
				}
			}
			ImGui::EndChild();
		}
		selectedFileStr = selectedFileKey;

		static int curr = 0;

		// 2. エディタ右側
		{
			ImGui::BeginGroup();
			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
			ImGui::Text("Selected File: %s", selectedFileKey);
			ImGui::Separator();
			// CSVファイルの中身を表示。
			static int currentLine = 0;
			{
				if (selectedFileKey != "") {
					int i = 0;
					auto sentence = ptr_csvContentManger.lock()->GetContent(selectedFileKey);
					auto st = sentence->GetCSVData();
					for (auto iter = st.begin(); iter != st.end(); ++iter) {

						auto s = [&]() {
							std::string s{ "" };
							for (auto& e : st[i]) s += e;
							return s;
						};
						ImGui::Selectable(s().c_str());
						i++;
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

void Teller::Editor::CallByParent()
{
}

void Teller::EpisodeEditor::Initialize()
{
}

void Teller::EpisodeEditor::CallByParent()
{
	ptr_csvContentManger = parent.lock()->GetCSVContentsManager();
}
