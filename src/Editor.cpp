#include"Editor.h"

using std::abs;
namespace util = ax::NodeEditor::Utilities;
using namespace ax;
using namespace ci::app;
using namespace teller;

using ax::Widgets::IconType;

void teller::TopLevelMenu::Tick()
{
	ImGui::Begin("Menu");
	ImGui::End();
}

void teller::TopLevelMenu::LoadFile(fs::path _path)
{
}

void teller::AssetViewer::Tick()
{
	ImGui::Begin("AssetViewer");

	ImGui::BeginChild("ContentsListChild", ImVec2(400, 0));

	if (ImGui::BeginTabBar("ContentsTabs")) {

		//�G�s�\�[�h�t�@�C���\��
		if (ImGui::BeginTabItem("Episode files")) {
			std::vector<fs::path> entries = cppglob::glob(episodePath_ / fs::path("*.csv"));
			for (auto& e : entries) {
				ImGui::Selectable(e.filename().string().c_str(), targetFile == e);
				if (ImGui::IsItemClicked())
					targetFile = e;
			}
			ImGui::EndTabItem();
		}

		//�L�����N�^�[�f�[�^�\��
		if (ImGui::BeginTabItem("Character visuals.")) {
			std::vector<fs::path> entries = cppglob::glob(characterPath_ / fs::path("*.json"));
			for (auto& e : entries) {
				ImGui::Selectable(e.filename().string().c_str(), targetFile == e);
				if (ImGui::IsItemClicked())
					targetFile = e;
			}
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("Asset Command");

	if (ImGui::Button("Load")) parent.lock()->LoadFileToEditor(targetFile);

	ImGui::EndChild();


	ImGui::End();
}

void teller::Editor::Tick()
{
	TickInternal();
}

void teller::Editor::TickInternal()
{
}

void teller::Editor::Update()
{
}

void teller::Editor::Save()
{
}

void teller::Editor::CallByParent()
{
}

void teller::Editor::LoadFile(fs::path _path)
{
}

bool teller::Editor::Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiID id = window->GetID("##Splitter");
	ImRect bb;
	bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
	bb.Max = bb.Min + ImGui::CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
	return ImGui::SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}

void teller::EpisodeEditor::Initialize()
{
}

void teller::EpisodeEditor::CallByParent()
{
}

void teller::EpisodeEditor::LoadFile(fs::path _path)
{
}

void teller::EpisodeEditor::Tick()
{
	namespace fs = std::filesystem;
	/*
	ImGui::Begin~----;
	ImGui::End~----;
	���̊Ԃɏ����������B
	*/

	ImGui::Begin("Episode Editor");

	static int selectedFile = 0;
	static int currentLine = 0;

	// 1. �G�f�B�^����
	{
		ImGui::BeginChild("left pane", ImVec2(150, 0), true);
		ImGui::Text("Loaded files.");
		//�f�B���N�g���ɑ��݂���t�@�C����\��
		{
			auto _path = fs::current_path();
			_path = _path.parent_path();
			_path /= fs::path("data\\csvdata");
			std::vector<fs::path> entries = cppglob::glob(_path / fs::path("*.csv"));
			if (entries.size() != 0)
				for (auto& e : entries)
					if (ImGui::Selectable(e.filename().string().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
						if (ImGui::IsMouseDoubleClicked(0)) CSVRef = std::make_unique<CSVLoader>(e);
		}
		ImGui::EndChild();
	}
	ImGui::SameLine();


	// 2. �G�f�B�^�E��
	{
		ImGui::BeginChild("item view", ImVec2(0, 0)); // Leave room for 1 line below us
		ImGui::Text("Selected File: ");
		ImGui::BeginChild("ite", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() * 4));
		ImGui::BeginGroup();
		// CSV�t�@�C���̒��g��\���B
		{
			if (CSVRef) {

				int i = 0;
				for (auto& line : CSVRef->GetCSVData()) {

					//std::vector<std::string>��std::string�ɓW�J

					if (lineBracket.first == i || lineBracket.second == i) {
						ImGui::Selectable(SingleLine(line.second).c_str(), true);
					}
					else {
						if (ImGui::Selectable(SingleLine(line.second).c_str(), currentLine == i, ImGuiSelectableFlags_AllowDoubleClick))
							if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) currentLine = i;
					}
					i++;
				}
			}
		}
		ImGui::EndGroup();
		ImGui::EndChild();

		ImGui::Separator();
		if (ImGui::Button("Set begin"))
			if (currentLine < lineBracket.second) lineBracket.first = currentLine;
		ImGui::SameLine();
		if (ImGui::Button("Set end"))
			if (lineBracket.second < currentLine) lineBracket.second = currentLine;

		ImGui::SameLine();
		ImGui::Text("Episode Name: It must be a Unique Name.");
		ImGui::InputText("", &episodeNameCandidate);

		if (ImGui::Button("Export") && episodeNameCandidate != "") {


			auto epMap = std::map<int, std::vector< std::string>>();

			{
				int ln = 0;
				for (auto i = lineBracket.first; i < lineBracket.second; i++) {
					epMap.emplace(ln, CSVRef->GetLine(i));
					ln++;
				}
			}

			//TODO:������֐��ɐ؂�o���B
			//csv�֏�������
			fs::path p = fs::current_path();
			p = p.parent_path();
			p /= fs::path("data");
			p /= fs::path("episodes");
			auto outFileName = episodeNameCandidate + ".csv";
			p /= fs::path(outFileName);
			std::cout << p.string() << std::endl;
			std::ofstream ofile(p.string());
			unsigned char bom[] = { 0xEF,0xBB,0xBF };
			ofile.write((char*)bom, sizeof(bom));
			for (auto i = 0; i < epMap.size(); i++) {
				ofile << epMap[i].at(0) << "," << epMap[i].at(1) << std::endl;
			}
			ofile.close();

			//parent.lock()->AddEpisode(_id, std::move(ne));
			//auto ne = std::make_unique<Episode>(outFileName);

#ifdef _DEBUG
			{
				int ln = 0;
				for (auto i = 0; i < epMap.size(); i++) {
					std::cout << epMap[i].at(0) << "," << epMap[i].at(1) << std::endl;
				}
			}
#endif // DEBUG

		}
		ImGui::EndChild();

	}
	ImGui::End();
}

void teller::EpisodeEventEditor::Initialize()
{
}

std::vector<std::string> teller::EpisodeEventEditor::GetSpritesName(json _cjson)
{
	auto vec = std::vector<std::string>();

	for (auto& e : _cjson["file"].items()) {
		vec.push_back(e.value().get<std::string>());
	}

	return vec;
}

void teller::EpisodeEventEditor::DrawLinks()
{
}

void teller::EpisodeEventEditor::NodeEditorTick()
{
	ImGui::Begin("EpisodeEventEditorNode");
	tnodeEditor->Tick();
	ImGui::End();
}

void teller::EpisodeEventEditor::LoadEpisodeEvent(json _j)
{
}

void teller::EpisodeEventEditor::SwapEvent(EventPack& _vector, int m, int n)
{
	EventPack::iterator it1;
	EventPack::iterator it2;
	it1 = _vector.begin();
	it2 = _vector.begin();

	std::advance(it1, m);
	std::advance(it2, n);

	std::iter_swap(it1, it2);
}



void teller::EpisodeEventEditor::Tick()
{
	if (!bEnabled) return;

	//TODO:�L�����N�^�[�����ڕύX�p�G�f�B�^�[�ƃm�[�h�G�f�B�^��؂�o���B
	//�J�n
	ImGui::Begin("Character appearance", 0, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar()) {

		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("save"))
				Save();

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	//�G�s�\�[�h�t�@�C���̃e�L�X�g�\��
	ShowEpisodeText();

	ImGui::SameLine();

	ImGui::BeginChild("Event sprite", ImVec2(300, 0));

	//TODO:�֐��֐؂�o��
	//�L�����N�^�[�̉摜�ؑ�
	if (episodeRef) {
		//�G�s�\�[�h�t�@�C����1��ڂ��b��
		auto& characterName = episodeRef->data[currentLine].at(0);
		ImGui::Text(("Name :" + characterName).c_str());
		int i = 0;
		if (ImGui::BeginTabBar("Simple Event Tabs")) {
			if (ImGui::BeginTabItem("Appearance")) {
				ImGui::Text("Left click : Make an event  Right click : Preview Appearance");
				if (characterName != "") {

					if (ImGui::Selectable("Inherit")) {
						//TODO:�O�̃C�x���g�̏��������p���Ƃ��̏����B
						characterAppearanceNum = 0;
					}
					auto filevec = GetSpritesName(jsonCharacterMap[characterName]);
					for (auto& e : filevec) {
						ImGui::Selectable(e.c_str(), characterAppearanceNum == i);

						//�E�N���b�N�Ńv���r���[
						if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
							characterAppearanceNum = i;
							previewAnimationMap[characterName]->Change(e);
						}

						//���N���b�N�Ŋm��
						if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
							characterAppearanceNum = i;
							previewAnimationMap[characterName]->Change(e);
							CreateEpisodeEvent(EPISODE_EVENT_TYPE::CHANGE_CHARACTER_APPERANCE, currentLine, characterName, e);
						}

						i++;
					}
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("CharacterInOut")) {

				if (ImGui::BeginCombo("##CharacterCombo", "Choose a character.", ImGuiComboFlags_HeightRegular)) {

					//�L�����N�^�[�ꗗ(json���ǂݍ��߂�������\��)
					for (auto& e : previewCharacterMap)
						if (ImGui::Selectable(e.first.c_str()))
							targetCharacter = e.first;

					ImGui::EndCombo();
				}

				//�L�����N�^�[�o��C�x���g
				if (ImGui::Selectable("Character IN"))
					if (characterName != "")
						CreateEpisodeEvent(EPISODE_EVENT_TYPE::CHARACTER_IN, currentLine, targetCharacter, targetCharacter);

				//�L�����N�^�[�ޏ�C�x���g
				if (ImGui::Selectable("Character OUT"))
					if (characterName != "")
						CreateEpisodeEvent(EPISODE_EVENT_TYPE::CHARACTER_OUT, currentLine, targetCharacter, targetCharacter);

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	ImGui::EndChild();

	ShowPreview();

	ImGui::SameLine();

	//TODO:�֐��֐؂�o��
	ImGui::BeginChild("Events List");
	ImGui::Text("EventPack");

	{
		int index = 0;
		for (auto& e : eventPackMap[currentLine]) {
			auto slabel = std::string{ "" };
			switch (e->type_)
			{
			case EPISODE_EVENT_TYPE::CHANGE_CHARACTER_APPERANCE:
				slabel = "EVENT: Changes Appearance" + e->key_;
				break;

			case EPISODE_EVENT_TYPE::CHARACTER_IN:
				slabel = "EVENT: Character IN" + e->key_;
				break;

			case EPISODE_EVENT_TYPE::CHARACTER_OUT:
				slabel = "EVENT: Character OUT" + e->key_;
				break;

			default:
				break;
			}

			ImGui::Selectable(slabel.c_str(), eventPackNum == index);
			if (ImGui::IsItemClicked()) {
				eventPackNum = index;
				std::cout << "Clicked" << eventPackNum << std::endl;
			}

			index++;
		}
	}

	//TODO:eventPack.size()=0�̂Ƃ��̏�����ǉ�����
	//�C�x���g�Đ����̕ύX
	auto& evn = eventPackMap[currentLine];
	if (ImGui::Button("Up") && eventPackNum != 0) {
		SwapEvent(evn, eventPackNum, eventPackNum - 1);
		++eventPackNum;
	}

	ImGui::SameLine();

	if (ImGui::Button("Down") && eventPackNum <= evn.size() - 1) {
		SwapEvent(evn, eventPackNum, eventPackNum + 1);
		++eventPackNum;
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete selected event.") && eventPackMap[currentLine].size() > eventPackNum) {
		auto eventindex = eventPackMap[currentLine].begin();
		std::advance(eventindex, eventPackNum);
		eventPackMap[currentLine].erase(eventindex);
	}

	ImGui::EndChild();

	ImGui::End();

	NodeEditorTick();
}

void teller::EpisodeEventEditor::ShowEpisodeText()
{
	ImGui::BeginChild("EpisodeText", ImVec2(900, 0));
	//episodeRef!=nullptr�̂Ƃ��e�L�X�g�\���B
	if (episodeRef) {
		int i = 0;
		for (auto& e : episodeRef->data) {
			auto str = e.second.at(0) + e.second.at(1);
			if (ImGui::Selectable(str.c_str(), currentLine == i)) {
				currentLine = i;
				previewTextChanger->SetText(e.second.at(0), e.second.at(1));
			}
			i++;
		}
	}
	ImGui::EndChild();
}

void teller::EpisodeEventEditor::LoadFile(fs::path _path)
{
	LoadEpisode(_path);
	LoadCharacterJson(_path);
}


void teller::EpisodeEventEditor::Update()
{
}

void teller::EpisodeEventEditor::UpdateAssetList()
{
}

void teller::EpisodeEventEditor::Save()
{
	//�G�s�\�[�h�C�x���g��ۑ�����json�ɏ�������
	json j;
	std::ofstream o(jsonFilePath_);
	j["name"] = episodeRef->name_;

	for (auto& evntPack : eventPackMap) {
		int order = 0;
		for (auto& evnt : evntPack.second) {
			std::string type_;

			//FIXME:�Ȃ������܂��ۑ�����Ȃ��B
			//json["events"][�s�̎w��][�C�x���g���Đ����鏇��]
			//j["events"][evntPack.first][order]
			switch (evnt->type_)
			{
			case EPISODE_EVENT_TYPE::CHANGE_CHARACTER_APPERANCE:
				type_ = "CHANGE_CHARACTER_APPERANCE";
				j["events"][std::to_string(evntPack.first)][std::to_string(order)]["description"] = evnt->key_;
				break;

			case EPISODE_EVENT_TYPE::CHARACTER_IN:
				type_ = "CHARACTER_IN";
				break;

			case EPISODE_EVENT_TYPE::CHARACTER_OUT:
				type_ = "CHARACTER_OUT";
				break;

			default:
				break;
			}
			j["events"][std::to_string(evntPack.first)][std::to_string(order)]["type"] = type_;

			order++;
		}
	}
	o << j;
}

void teller::EpisodeEventEditor::CallByParent()
{
}

void teller::EpisodeEventEditor::LoadEpisode(fs::path _path)
{
	episodeRef = std::make_unique<Episode>(_path);
	jsonFilePath_ = _path;
	jsonFilePath_.replace_extension("json");

	if (fs::directory_entry(jsonFilePath_).exists()) {
		std::ifstream i(jsonFilePath_.string());
		i >> jsonEpisode;
		LoadEpisodeEvent(jsonEpisode);
	}
	else {
		std::ofstream o(jsonFilePath_.string());
		jsonEpisode["name"] = jsonFilePath_.stem().string();
		o << jsonEpisode;
	}
	std::cout << episodeRef->name_ << "Loaded" << std::endl;
}


void teller::EpisodeEventEditor::LoadCharacterJson(fs::path _path)
{
	//�o��l���N�����邩���� �d���Ȃ�
	std::set<std::string> cset;
	for (auto& e : episodeRef->data) {
		if (e.second.at(0) != "")
			cset.insert(e.second.at(0));
	}

	for (auto& e : cset)std::cout << e << std::endl;

	//�R�s�[�ł���
	auto jpath_ = _path.parent_path();
	jpath_ = jpath_.parent_path();
	jpath_ /= fs::path("images");

	auto jsonentries = cppglob::glob(jpath_ / fs::path("*\\CharacterData.json"));

	for (auto& e : jsonentries) {
		std::cout << e.string() << std::endl;
		json j;
		std::ifstream i(e);
		//�ǂݍ��߂Ȃ������玟
		if (!i) {
			std::cout << "Skipped :" << e.string() << std::endl;
			continue;
		}

		i >> j;

		auto nameInJson = j["name"].get<std::string>();

		std::cout << "Json Loaded :" << nameInJson << std::endl;
		jsonCharacterMap[nameInJson] = j;

		previewCharacterMap[nameInJson] = std::make_unique<CharacterSimple>(vec2(0), vec2(0), vec2(0), e, nameInJson);
		previewAnimationMap[nameInJson] = std::make_unique<CharacterAppearanceChanger>();
		previewAnimationMap[nameInJson]->AttachToAgent(previewCharacterMap[nameInJson]);
	}
}

void teller::EpisodeEventEditor::CreateEpisodeEvent(EPISODE_EVENT_TYPE _type, int _line, std::string _target, std::string _key)
{
	eventPackMap[_line].push_back(std::make_unique<EpisodeEvent>(_type, _line, _target, _key));
}


void teller::AssetViewer::Initialize()
{
	episodePath_ = fs::current_path();
	episodePath_ = episodePath_.parent_path();
	episodePath_ /= fs::path("data\\episodes");

	characterPath_ = fs::current_path();
	characterPath_ = characterPath_.parent_path();
	characterPath_ /= fs::path("data\\images");

	dataPath_ = fs::current_path();
	dataPath_ = dataPath_.parent_path();

}


void teller::AssetViewer::CallByParent()
{
}

void teller::AssetViewer::LoadFile(fs::path _path)
{
}

void teller::EditorManager::EditorCallBack(std::string _filename)
{
}

void teller::EditorManager::EditorCallBack(std::unique_ptr<Episode> _episode)
{
}

std::string teller::EpisodeEditor::SingleLine(std::vector<std::string > _vector) {
	auto s = std::string("");
	for (auto& e : _vector)
		s += e;

	return s;
}

void teller::EpisodeEventEditor::ShowPreview()
{
	if (previewCharacterMap.size() != 0)
		for (auto& e : previewCharacterMap) {
			e.second->Tick();
		}

	previewText->Tick();
}

void teller::CharacterEditor::Initialize(fs::path _path)
{
}

fs::path teller::CharacterEditor::OpenFile()
{
	return fs::path();
}

void teller::CharacterEditor::Tick()
{
}

void teller::CharacterEditor::LoadFile(fs::path _path)
{
}

void teller::EpisodeEventNodeEditor::Initialize()
{
}

void teller::EpisodeEventNodeEditor::DrawInputSocketInternal()
{
}

void teller::EpisodeEventNodeEditor::DrawOutputSocketInternal()
{
}

void teller::EpisodeEventNodeEditor::LoadFile(fs::path _path)
{
}

void teller::EpisodeEventNodeEditor::AddNodeSignature(TNodeSignature<Episode_Event_Node, Socket_Data_Type> _nodesig)
{
}
