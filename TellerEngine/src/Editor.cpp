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
			auto sentence = ptr_csvContentManger.lock()->GetContent(fileVec_.at(selectedFile));
			auto st = sentence->GetCSVData();
			int i = 0;
			for (auto iter = st.begin(); iter != st.end(); ++iter) {

				auto s = [=]() {
					auto ts = std::string("");
					for (auto& e : iter->second) ts += e;
					return ts;
				};
				if (ImGui::Selectable(s().c_str(), currentLine == i))currentLine = i;
				i++;
			}
		}
		ImGui::EndChild();
		if (ImGui::Button("Revert")) {}
		ImGui::SameLine();
		if (ImGui::Button("Save")) {}
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
