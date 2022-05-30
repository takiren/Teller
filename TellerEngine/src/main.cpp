#include <io.h>
#include <Fcntl.h>
#include<Windows.h>
#include <list>

#include<cinder/Cinder.h>
#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include<cinder/ImageIo.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include <cinder/Rand.h>
#include<cinder/Log.h>
#include <imgui_node_editor.h>

#include"japaneseGryph.h"

#include"TellerCore.h"
#include"Game.h"
#include"Scene.h"
#include"Agent.h"
#include"Animation.h"


using namespace ci;
using namespace ci::app;
using namespace Teller;
namespace fs = std::filesystem;
// We'll create a new Cinder Application by deriving from the App class
namespace ed = ax::NodeEditor;

static ed::EditorContext* g_Context = nullptr;

class BasicAppMultiWindow : public App {
public:
	std::shared_ptr<TellerCore> mCore;
	std::vector<std::string> testvec{ 5,"test" };

	void setup();
	void createNewWindow();
	void update() override;
	void mouseDrag(MouseEvent event);
	void keyDown(KeyEvent event);
	void draw();
};

// The window-specific data for each window
class WindowData {
public:
	WindowData()
		: mColor(Color(CM_HSV, randFloat(), 0.8f, 0.8f)) // a random color
	{}

	Color mColor;
};

void BasicAppMultiWindow::setup()
{
	g_Context = ed::CreateEditor();
	setlocale(LC_CTYPE, "");
	// for the default window we need to provide an instance of WindowData
	getWindow()->setUserData(new WindowData);
	ImGui::Initialize();
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("../data/ipaexg.ttf", 14.0f, nullptr, glyphRangesJapanese);
	}

	auto CMCSV = std::make_shared<CSVManager>();
	auto CMSprite = std::make_shared<SpriteManager>();
	auto CMEpisode = std::make_shared<EpisodeManager>();

	mCore = std::make_shared<TellerCore>(
		CMSprite,
		CMEpisode,
		CMCSV);

	/*mCore->AttachDeltaTimeMessanger(0,
		[&](float deltaTime) { mAnimator->SetDeltaTime(deltaTime); }
	);*/

	auto mGame = std::make_shared<GameModule>();
	auto mScene = std::make_shared<SceneModule>();
	auto mAgent = std::make_shared<RectAgent>();

	mCore->AddModule(mGame);
	mGame->AddChildModule(mScene);
	mScene->AddAgent(mAgent);

	auto animSeq = std::make_unique<AnimationSequencer>("args");

	auto mAnimator = std::make_unique<Circular>();

	auto textAgent = std::make_shared<MainTextArea>();
	mScene->AddAgent(textAgent);
	auto tChanger = std::make_unique<TextChanger>();

	auto cmcs = std::make_unique<ContentsManager<CSVLoader>>();

	CMCSV->LoadContent("../data/story.csv");
	CMCSV->LoadContent("../data/episode.csv");

	tChanger->AttachToAgent(textAgent);
	tChanger->LoadCSV("../data/story.csv");
	mAnimator->AttachToAgent(mAgent);
	animSeq->AddAnimator(std::move(mAnimator));
	animSeq->AddAnimator(std::move(tChanger));
	mCore->AddAnimSequencer(std::move(animSeq));

	auto ed = std::make_shared<EpisodeEditor>();
	mCore->AddEditor(ed);

	auto ne = std::make_unique<EpisodeEventEditor>();
	mCore->AddEditor(std::move(ne));
	//��Âɍl������|�C���^�ǂ��������Ă邩��폜�B
	/*setvbuf(stdout, NULL, _IONBF, 0);

	AllocConsole();
	auto hConsole = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	*stdout = *_fdopen(hConsole, "w");*/

	ci::app::setWindowSize(1280, 720);
	ci::app::setWindowPos(vec2(1920 / 2 - 1280 / 2, 1080 / 2 - 720 / 2));
}

void BasicAppMultiWindow::createNewWindow()
{
	app::WindowRef newWindow = createWindow(Window::Format().size(400, 400));
	newWindow->setUserData(new WindowData);

	// for demonstration purposes, we'll connect a lambda unique to this window which fires on close
	int uniqueId = getNumWindows();
	newWindow->getSignalClose().connect(
		[uniqueId, this] { this->console() << "You closed window #" << uniqueId << std::endl; }
	);
}

void BasicAppMultiWindow::update()
{
}

void BasicAppMultiWindow::mouseDrag(MouseEvent event)
{
	WindowData* data = getWindow()->getUserData<WindowData>();
}

void BasicAppMultiWindow::keyDown(KeyEvent event)
{
}

void BasicAppMultiWindow::draw()
{
	gl::clear(Color(0.1f, 0.1f, 0.15f));
	gl::enableAlphaBlending();

	/*
	�����L�q��������
	*/
	mCore->Tick();
	ed::SetCurrentEditor(g_Context);

	ed::Begin("My Editor");

	int uniqueId = 1;

	// Start drawing nodes.
	ed::BeginNode(uniqueId++);
	ImGui::Text("Node A");
	ed::BeginPin(uniqueId++, ed::PinKind::Input);
	ImGui::Text("-> In");
	ed::EndPin();
	ImGui::SameLine();
	ed::BeginPin(uniqueId++, ed::PinKind::Output);
	ImGui::Text("Out ->");
	ed::EndPin();
	ed::EndNode();

	ed::End();
	/*
	�����L�q�����܂�
	*/

	gl::end();
}

// This line tells Cinder to actually create the application
CINDER_APP(BasicAppMultiWindow, RendererGl)