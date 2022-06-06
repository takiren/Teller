#include"Editor.h"

using std::abs;
namespace util = ax::NodeEditor::Utilities;
using namespace ax;
using namespace ci::app;

using ax::Widgets::IconType;

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

	ImGui::Begin("Episode Editor");

	static int selectedFile = 0;
	static int currentLine = 0;

	// 1. エディタ左側
	{
		ImGui::BeginChild("left pane", ImVec2(150, 0), true);
		ImGui::Text("Loaded files.");
		// ロードされたファイルを左側に表示。
		{
			int i = 0;
			if (fileVec_.size() != 0) {
				for (auto& e : fileVec_) {
					auto p = ptr_csvContentManger.lock()->GetContent(e);
					if (ImGui::Selectable(p->path_.c_str(), selectedFile == i)) {
						selectedFile = i;
					}
					i++;
				}
			}
		}
		ImGui::EndChild();
	}
	ImGui::SameLine();

	// 2. エディタ右側
	{
		ImGui::BeginChild("item view", ImVec2(0, 0)); // Leave room for 1 line below us
		ImGui::Text("Selected File: %s", fileVec_.at(selectedFile));
		ImGui::BeginChild("ite", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() * 4));

		ImGui::BeginGroup();
		// CSVファイルの中身を表示。
		{
			auto cont = ptr_csvContentManger.lock()->GetContent(fileVec_.at(selectedFile));
			auto st = cont->GetCSVData();
			int i = 0;
			for (auto iter = st.begin(); iter != st.end(); ++iter) {

				//std::vector<std::string>をstd::stringに展開
				auto s = [=]() {
					std::string ts("");
					for (auto& e : iter->second) ts += e;
					return ts;
				};
				if (lineBracket.first == i || lineBracket.second == i) {
					ImGui::Selectable(s().c_str(), true);
				}
				else {
					if (ImGui::Selectable(s().c_str(), currentLine == i, ImGuiSelectableFlags_AllowDoubleClick))
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) currentLine = i;
				}
				i++;
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
			auto cont = ptr_csvContentManger.lock()->GetContent(fileVec_.at(selectedFile));
			{
				int ln = 0;
				for (auto i = lineBracket.first; i < lineBracket.second; i++) {
					epMap.emplace(ln, cont->GetLine(i));
					ln++;
				}
			}

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

void Teller::AssetViewer::Tick()
{
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

void Teller::EpisodeEditor::Initialize()
{
}

void Teller::EpisodeEditor::CallByParent()
{
	ptr_csvContentManger = parent.lock()->GetCSVContentsManager();
	fileVec_ = ptr_csvContentManger.lock()->GetKeys();
	ptr_episodeContentManager = parent.lock()->GetEpisodeContentManager();
}

void Teller::EpisodeEventEditor::CallByParent()
{
	ptrEpsdMngr = parent.lock()->GetEpisodeContentManager();
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
	panewidth = ImGui::GetContentRegionAvailWidth();
	ImGui::BeginHorizontal("Editor", ImVec2(panewidth, 0));
	ImGui::Spring(0.0f);

	if (ImGui::Button("Add Node")) {

	}
	ImGui::Spring();
	if (ImGui::Button("style")) {

	}
	ImGui::EndHorizontal();
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

void Teller::EpisodeEventEditor::Update()
{
}

void Teller::EpisodeEventEditor::Tick()
{
	if (!bEnabled) return;
	ImGui::Begin("EpisodeEventEditor");
	//ShowLeftPane(150);

	OpenAddNodePopup();
	auto openPopupPosition = ImGui::GetMousePos();
	int selected = -1;
	if (ImGui::BeginPopup("AddNode")) {
		ImGui::Text("Node list.");

		// 追加可能なノードリスト
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
		// 関数ポインタでもっとスマートな実装にしてもいいかも。
		uint64_t id_;
		switch (selected)
		{
		case 0:
			id_ = ptrTNodeManager->AddTNodeBranch();
			break;
		case 1:
			id_ = ptrTNodeManager->AddTNodeSceneChange();
			break;
		case 2:
			id_ = ptrTNodeManager->AddTNodeAnimation();
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

	// 1.ノードマネージャーから読み取って描画
	{
		util::BlueprintNodeBuilder builder;

		// ノードでイテレーション
		for (auto& node : ptrTNodeManager->nodes) {

			builder.Begin(node.second->ID_);

			//builderでの操作。
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

				// インプットソケットの描画
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

				// アウトプットソケットの描画
				{
					auto alpha = ImGui::GetStyle().Alpha;
					for (auto& e : node.second->socketsOutput) {
						ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
						builder.Output(e->ID_);
						ImGui::Spring(0);
						ImGui::TextUnformatted(node.second->title_.c_str());
						// ピンごとの条件分岐を記述ここから

						// ここまで
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
	// 2.リンクの描画
	{

	}

	ed::End();
	ed::SetCurrentEditor(nullptr);
	ImGui::End();
}

Teller::NodeEditorBase::NodeEditorBase(std::string _name)
{
}

void Teller::NodeEditorBase::Tick()
{
	ImGui::Begin(name.c_str());

	ed::Begin("Editor");

	ed::End();
	ImGui::End();
}

void Teller::SequenceEditor::CallByParent()
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
	ImGui::Begin(name.c_str());
	ImGui::BeginChild("Episode list.");
	for (auto& e : episodeMap) {
		if (ImGui::Selectable(e.second.c_str())) {
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))ptrTNodeManager->AddEpisodeNode(e.first);
		}
	}
	ImGui::EndChild();

	// ノードマネージャーからノードを描画。

	ed::Begin("Sequence editor.");
	util::BlueprintNodeBuilder builder;
	for (auto& node : ptrTNodeManager->nodes)
	{
		builder.Begin(node.second->ID_);

		//builderでの操作。
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

			// インプットソケットの描画
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

			// アウトプットソケットの描画
			{
				auto alpha = ImGui::GetStyle().Alpha;
				for (auto& e : node.second->socketsOutput) {
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
					builder.Output(e->ID_);
					ImGui::Spring(0);
					ImGui::TextUnformatted(node.second->title_.c_str());
					// ピンごとの条件分岐を記述ここから

					// ここまで
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

void Teller::CharacterEditor::Initialize(fs::path _path) {
	
}