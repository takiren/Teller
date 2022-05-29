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
	���̊Ԃɏ����������B
	*/

	ImGui::SetNextWindowSize(ImVec2(200, 440), ImGuiCond_FirstUseEver);

	ImGui::Begin("Episode Editor");

	static int selectedFile = 0;
	static int currentLine = 0;

	// 1. �G�f�B�^����
	{
		ImGui::BeginChild("left pane", ImVec2(150, 0), true);
		ImGui::Text("Loaded files.");
		// ���[�h���ꂽ�t�@�C���������ɕ\���B
		{

			int i = 0;
			if (fileVec_.size() != 0) {
				for (auto& e : fileVec_) {
					if (ImGui::Selectable(e.c_str(), selectedFile == i)) {
						selectedFile = i;
					}
					i++;
				}
			}
		}
		ImGui::EndChild();
	}
	ImGui::SameLine();

	// 2. �G�f�B�^�E��
	{
		ImGui::BeginGroup();
		ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
		ImGui::Text("Selected File: %s", fileVec_.at(selectedFile).c_str());
		ImGui::Separator();
		// CSV�t�@�C���̒��g��\���B
		{
			auto cont = ptr_csvContentManger.lock()->GetContent(fileVec_.at(selectedFile));
			auto st = cont->GetCSVData();
			int i = 0;
			for (auto iter = st.begin(); iter != st.end(); ++iter) {

				//std::vector<std::string>��std::string�ɓW�J
				auto s = [=]() {
					std::string ts("");
					for (auto& e : iter->second) ts += e;
					return ts;
				};
				if (ImGui::Selectable(s().c_str(), currentLine == i))currentLine = i;
				i++;
			}
		}
		ImGui::EndChild();
		if (ImGui::Button("Set")) {
			if (lineBracket.first < currentLine) {
				lineBracket.second = currentLine;
			}
			else {
				lineBracket.second = lineBracket.first;
				lineBracket.first = currentLine;
			}
		}
		ImGui::SameLine();
		ImGui::InputText("Episode Name: It must be a Unique Name.", &episodeNameCandidate);

		if (ImGui::Button("Save")) {
			if (episodeNameCandidate == "") {
			}
			else {

				auto epMap = std::map<int, std::vector< std::string>>();
				auto cont = ptr_csvContentManger.lock()->GetContent(fileVec_.at(selectedFile));
				auto csv_ = cont->GetCSVData();
				for (auto i = lineBracket.first; i < lineBracket.second; i++) {
					epMap.emplace(i, csv_[i]);
					parent.lock()->AddEpisode(episodeNameCandidate, std::move(std::make_unique<Episode>(epMap)));
				}

				#ifdef _DEBUG
				for (auto iter = epMap.begin(); iter != epMap.end(); ++iter) {
					std::cout << (SingleLine(iter->second).c_str()) << std::endl;
				}
				#endif // DEBUG
			}

		}
		ImGui::EndGroup();

	}
	ImGui::End();
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
	fileVec_ = ptr_csvContentManger.lock()->GetKeys();
}
