#include"Editor.h"

using std::abs;
namespace util = ax::NodeEditor::Utilities;
using namespace ax;
using namespace ci::app;

using ax::Widgets::IconType;

void Teller::TopLevelMenu::Tick()
{
	ImGui::Begin("Menu");
	ImGui::End();
}

void Teller::TopLevelMenu::LoadFile(fs::path _path)
{
}


void Teller::AssetViewer::Tick()
{
	ImGui::Begin("AssetViewer");

	ImGui::BeginChild("ContentsListdddd", ImVec2(400, 0));

	if (ImGui::BeginTabBar("Contentsdffd")) {

		if (ImGui::BeginTabItem("Episode files")) {
			std::vector<fs::path> entries = cppglob::glob(episodePath_ / fs::path("*.csv"));
			//FIXME:�_�u���N���b�N����ƑS���ǂݍ��܂��B���삪�s���R�������������̂����C�u�����������̂��s��
			for (auto& e : entries)
				if (ImGui::Selectable(e.filename().string().c_str()))
					targetFile = e;

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Character visuals.")) {
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

void Teller::Editor::Tick()
{
	TickInternal();
}

void Teller::Editor::TickInternal()
{
}

void Teller::Editor::Update()
{
}

void Teller::Editor::Save()
{
}

void Teller::Editor::CallByParent()
{
}

bool Teller::Editor::Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiID id = window->GetID("##Splitter");
	ImRect bb;
	bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
	bb.Max = bb.Min + ImGui::CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
	return ImGui::SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}

void Teller::EpisodeEditor::Initialize()
{
}

void Teller::EpisodeEditor::CallByParent()
{
}

bool Teller::EpisodeEditor::CanAccept(fs::path _path)
{
	return false;
}

void Teller::EpisodeEditor::LoadFile(fs::path _path)
{
}

void Teller::EpisodeEditor::Tick()
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
		ImGui::Text("Selected File: %s");
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

void Teller::EpisodeEventEditor::Initialize()
{
}


void Teller::EpisodeEventEditor::OpenAddNodePopup()
{
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
		DEBUG_PRINTF("Right clicked.");
		ImGui::OpenPopup("AddNode");
	}
}

void Teller::EpisodeEventEditor::ShowLeftPane(float panewidth)
{
	auto& io = ImGui::GetIO();
	ImGui::BeginChild("EventEditor", ImVec2(panewidth, 0));

	ImGui::EndChild();
}

ImColor Teller::EpisodeEventEditor::GetIconColor(Socket_TYPE type)
{
	switch (type)
	{
	case Teller::Socket_TYPE::Delegate:	return ImColor(255, 255, 255);
	case Teller::Socket_TYPE::BOOL:		return ImColor(220, 48, 48);
	case Teller::Socket_TYPE::INT:		return ImColor(68, 201, 156);
	case Teller::Socket_TYPE::OPTION:	return ImColor(147, 226, 74);
	case Teller::Socket_TYPE::FLOW:		return ImColor(255, 255, 255);
	default:							return ImColor(0, 0, 0);
	}
}

void Teller::EpisodeEventEditor::LoadFile(fs::path _path)
{
	LoadEpisode(_path);
	LoadCharacterJson(_path);
}


void Teller::EpisodeEventEditor::Update()
{
}

void Teller::EpisodeEventEditor::UpdateAssetList()
{
}

bool Teller::EpisodeEventEditor::CanAccept(fs::path _path)
{

	return false;
}


void Teller::EpisodeEventEditor::LoadEpisode(fs::path _path)
{
	episodeRef = std::make_unique<Episode>(_path);
	_path.replace_extension("json");
	if (fs::directory_entry(_path).exists()) {
		std::ifstream i(_path.string());
		i >> jsonEpisode;
	}
	else {
		std::ofstream o(_path.string());
		jsonEpisode["name"] = _path.stem().string();
		o << jsonEpisode;
	}
	std::cout << episodeRef->name_ << "Loaded" << std::endl;
}

void Teller::EpisodeEventEditor::CallByParent() {

}

void Teller::EpisodeEventEditor::LoadCharacterJson(fs::path _path)
{
	//�o��l���N�����邩���� �d���Ȃ�
	std::set<std::string> cset;
	for (auto& e : episodeRef->data) {
		if (e.second.at(0) != "")
			cset.insert(e.second.at(0));
	}

	for (auto& e : cset) std::cout << e << std::endl;
	return;

	for (auto& e : cset) {
		_path = _path.parent_path();
		_path /= fs::path("images") / fs::path(e) / fs::path("CharacterData.json");

		if (fs::directory_entry(_path).exists()) {
			json j;
			std::ifstream i(_path);
			i >> j;
			jsonCharacterMap[e] = j;
			std::cout << "Character Json Loaded. : " << j["name"] << std::endl;
		}
		else std::cout << "Skipped : " << e << std::endl;
	}
}

void Teller::EpisodeEventEditor::Tick()
{
	if (!bEnabled) return;

	//�L�����N�^�[�̉摜�ؑ�
	ImGui::Begin("Character appearance");
	ImGui::BeginChild("EpisodeText", ImVec2(1000, 0));

	//episodeRef!=nullptr�̂Ƃ��e�L�X�g�\���B
	if (episodeRef) {
		int i = 0;
		for (auto& e : episodeRef->data) {
			auto str = e.second.at(0) + e.second.at(1);
			if (ImGui::Selectable(str.c_str()), currentLine == i)
				currentLine = i;
			i++;
		}
	}

	ImGui::EndChild();
	ImGui::SameLine();

	ImGui::BeginChild("File list");
	if (episodeRef) {
		//�G�s�\�[�h�t�@�C����1��ڂ��b��
		auto& characterName = episodeRef->data[currentLine].at(0);
		int i = 0;
		if (characterName != "")
			for (auto& e : jsonCharacterMap[characterName]["file"].items()) {
				if (ImGui::Selectable(e.value().dump().c_str(), characterAppearanceNum == i)) {
					characterAppearanceNum = i;
					eventsMap[currentLine] = std::move(std::make_unique<EpisodeEvent>(EPISODE_EVENT_TYPE::CHANGE_CHARACTER_APPERANCE, e.value().dump()));
				}
				i++;
			}
	}

	ImGui::EndChild();

	ImGui::End();

	//�m�[�h�G�f�B�^
	ImGui::Begin("EpisodeEventEditor");

	ImGui::BeginChild("NodeEditor Event");
	OpenAddNodePopup();
	auto openPopupPosition = ImGui::GetMousePos();
	int selected = -1;
	if (ImGui::BeginPopup("AddNode")) {
		ImGui::Text("Node list.");

		// �ǉ��\�ȃm�[�h���X�g
		ImGui::Separator();
		int i = 0;
		for (auto& e : nodeList_) {
			if (ImGui::Selectable(e.c_str())) {
				selected = i;
			}
			i++;
		}
		ImGui::EndPopup();
	}

	if (selected != -1) {
		// �֐��|�C���^�ł����ƃX�}�[�g�Ȏ����ɂ��Ă����������B
		uint64_t id_;
		switch (selected)
		{
		case 0:
			//����
			id_ = TNodeManagerRef->AddTNodeBranch();
			break;
		case 1:
			//�V�[���`�F���W
			id_ = TNodeManagerRef->AddTNodeSceneChange();
			break;
		case 2:
			id_ = TNodeManagerRef->AddTNodeAnimation();
			break;

		case 6:
			id_ = TNodeManagerRef->AddTNodeCharacterInOut();
			break;
		default:
			DEBUG_PRINTF("Nothing added.");
			break;
		}
		//ed::SetNodePosition(id_, openPopupPosition);
	}

	ed::SetCurrentEditor(gContext);
	auto cursorTopLeft = ImGui::GetCursorScreenPos();

	ed::Begin("Editor");

	// 1.�m�[�h�}�l�[�W���[����ǂݎ���ĕ`��
	{
		util::BlueprintNodeBuilder builder;

		// �m�[�h�ŃC�e���[�V����
		for (auto& node : TNodeManagerRef->nodes) {

			builder.Begin(node.second->ID_);

			//builder�ł̑���B
			{
				builder.Header(ImColor(255, 255, 255));
				{
					ImGui::Spring(0);
					ImGui::TextUnformatted(node.second->title_.c_str());
					ImGui::Spring(1);
					ImGui::Dummy(ImVec2(0, 28));
					ImGui::Spring(0);
				}
				builder.EndHeader();

				// �C���v�b�g�\�P�b�g�̕`��
				{
					auto alpha = ImGui::GetStyle().Alpha;
					for (auto& e : node.second->socketsInput) {

						builder.Input(e->ID_);
						ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
						DrawPinIcon(e, false, (int)(alpha * 255));
						ImGui::Spring(0);
						ImGui::Spring(0);
						ImGui::PopStyleVar();
						builder.EndInput();
					}
				}

				// �A�E�g�v�b�g�\�P�b�g�̕`��
				{
					auto alpha = ImGui::GetStyle().Alpha;
					for (auto& e : node.second->socketsOutput) {
						ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
						builder.Output(e->ID_);
						ImGui::Spring(0);
						ImGui::TextUnformatted(node.second->title_.c_str());
						// �s�����Ƃ̏���������L�q��������

						// �����܂�
						ImGui::Spring(0);
						DrawPinIcon(e, false, (int)(alpha * 255));
						ImGui::PopStyleVar();
						builder.EndOutput();
					}
				}
			}
			builder.End();
		}
	}
	// 2.�����N�̕`��
	{

	}

	ed::End();
	ed::SetCurrentEditor(nullptr);
	ImGui::EndChild();

	ImGui::End();
}

void Teller::NodeEditorBase::Tick()
{
	ImGui::Begin(name_.c_str());

	ed::Begin("Editor");

	ed::End();
	ImGui::End();
}

void Teller::NodeEditorBase::LoadFile(fs::path _path)
{
}

void Teller::EpisodeEventEditor::DrawPinIcon(const std::shared_ptr<TSocketCore> sckt, bool connected, int alpha)
{
	IconType iconType;
	ImColor  color = GetIconColor(sckt->type_);
	color.Value.w = alpha / 255.0f;

	switch (sckt->type_)
	{
	case  Teller::Socket_TYPE::FLOW:		iconType = IconType::Flow;   break;
	case Teller::Socket_TYPE::BOOL:			iconType = IconType::Circle; break;
	case Teller::Socket_TYPE::INT:			iconType = IconType::Circle; break;
	case Teller::Socket_TYPE::OPTION:		iconType = IconType::Circle; break;
	default:
		return;
	}

	ax::Widgets::Icon(ImVec2(s_PinIconSize, s_PinIconSize), iconType, connected, color, ImColor(32, 32, 32, alpha));
};

void Teller::SequenceEditor::LoadFile(fs::path _path)
{
}

void Teller::SequenceEditor::UpdateEpisodeList()
{
	auto eplist = ptrEPCM.lock()->GetKeys();
	for (auto& key : eplist) {
		episodeMap[key] = ptrEPCM.lock()->GetContent(key)->title;
	}
}

void Teller::SequenceEditor::Tick()
{
	ImGui::Begin(name_.c_str());
	ImGui::BeginChild("Episode list.");
	for (auto& e : episodeMap) {
		if (ImGui::Selectable(e.second.c_str())) {
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))TNodeManagerRef->AddEpisodeNode(e.first);
		}
	}
	ImGui::EndChild();

	// �m�[�h�}�l�[�W���[����m�[�h��`��B

	ed::Begin("Sequence editor.");
	util::BlueprintNodeBuilder builder;
	for (auto& node : TNodeManagerRef->nodes)
	{
		builder.Begin(node.second->ID_);

		//builder�ł̑���B
		{
			builder.Header(ImColor(255, 255, 255));
			{
				ImGui::Spring(0);
				ImGui::TextUnformatted(node.second->title_.c_str());
				ImGui::Spring(1);
				ImGui::Dummy(ImVec2(0, 28));
				ImGui::Spring(0);
			}
			builder.EndHeader();

			// �C���v�b�g�\�P�b�g�̕`��
			{
				auto alpha = ImGui::GetStyle().Alpha;
				for (auto& e : node.second->socketsInput) {

					builder.Input(e->ID_);
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
					DrawPinIcon(e, false, (int)(alpha * 255));
					ImGui::Spring(0);
					ImGui::Spring(0);
					ImGui::PopStyleVar();
					builder.EndInput();
				}
			}

			// �A�E�g�v�b�g�\�P�b�g�̕`��
			{
				auto alpha = ImGui::GetStyle().Alpha;
				for (auto& e : node.second->socketsOutput) {
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
					builder.Output(e->ID_);
					ImGui::Spring(0);
					ImGui::TextUnformatted(node.second->title_.c_str());
					// �s�����Ƃ̏���������L�q��������

					// �����܂�
					ImGui::Spring(0);
					DrawPinIcon(e, false, (int)(alpha * 255));
					ImGui::PopStyleVar();
					builder.EndOutput();
				}
			}
		}
		builder.End();
	}

	ed::End();

	ImGui::End();
}

void Teller::SequenceEditor::Initialize()
{
	ptrEPCM = parent.lock()->GetEpisodeContentManager();
}

void Teller::SequenceEditor::callBackFromCSVManager(std::vector<std::string> _episode)
{
}

void Teller::SequenceEditor::DrawPinIcon(const std::shared_ptr<TSocketCore> sckt, bool connected, int alpha)
{
	IconType iconType;
	ImColor  color = GetIconColor(sckt->type_);
	color.Value.w = alpha / 255.0f;

	switch (sckt->type_)
	{
	case  Teller::Socket_TYPE::FLOW:		iconType = IconType::Flow;   break;
	case Teller::Socket_TYPE::BOOL:			iconType = IconType::Circle; break;
	case Teller::Socket_TYPE::INT:			iconType = IconType::Circle; break;
	case Teller::Socket_TYPE::OPTION:		iconType = IconType::Circle; break;
	default:
		return;
	}

	ax::Widgets::Icon(ImVec2(s_PinIconSize, s_PinIconSize), iconType, connected, color, ImColor(32, 32, 32, alpha));
};
ImColor Teller::SequenceEditor::GetIconColor(Socket_TYPE type)
{
	switch (type)
	{
	case Teller::Socket_TYPE::Delegate:	return ImColor(255, 255, 255);
	case Teller::Socket_TYPE::BOOL:		return ImColor(220, 48, 48);
	case Teller::Socket_TYPE::INT:		return ImColor(68, 201, 156);
	case Teller::Socket_TYPE::OPTION:	return ImColor(147, 226, 74);
	case Teller::Socket_TYPE::FLOW:		return ImColor(255, 255, 255);
	default:							return ImColor(0, 0, 0);
	}
}

void Teller::CharacterEditor::Initialize(fs::path _path)
{

}

std::filesystem::path Teller::CharacterEditor::OpenFile()
{
	return getOpenFilePath("");
}

void Teller::CharacterEditor::Tick()
{
}

void Teller::CharacterEditor::LoadFile(fs::path _path)
{
}

void Teller::AssetViewer::Initialize()
{
	episodePath_ = fs::current_path();
	episodePath_ = episodePath_.parent_path();
	episodePath_ /= fs::path("data\\episodes");

	characterPath_ = fs::current_path();
	characterPath_ = characterPath_.parent_path();
	characterPath_ /= fs::path("data\\images");
}

bool Teller::AssetViewer::CanAccept(fs::path _path)
{
	return false;
}

void Teller::AssetViewer::CallByParent()
{
}

void Teller::AssetViewer::LoadFile(fs::path _path)
{
}

void Teller::EditorManager::EditorCallBack(std::string _filename)
{
}

void Teller::EditorManager::EditorCallBack(std::unique_ptr<Episode> _episode)
{
}

std::string Teller::EpisodeEditor::SingleLine(std::vector<std::string > _vector) {
	auto s = std::string("");
	for (auto& e : _vector)s += e;
	return s;
}