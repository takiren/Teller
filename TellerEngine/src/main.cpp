#include<Windows.h>
#include<stdio.h>
#include <io.h>
#include <Fcntl.h>
#include <list>

#include<cinder/Cinder.h>
#include<cinder/CinderImGui.h>
#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include<cinder/ImageIo.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include <cinder/Rand.h>
#include<cinder/Log.h>

#include"japaneseGryph.h"

#include"TellerCore.h"
#include"Game.h"
#include"Scene.h"
#include"Agent.h"
#include"Animation.h"
#include"TellerEvent.h"

using namespace ci;
using namespace ci::app;
using namespace teller;
namespace fs = std::filesystem;
// We'll create a new Cinder Application by deriving from the App class

class TellerEngineMain : public App {
private:
	void ShowConsole();
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

void TellerEngineMain::ShowConsole()
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONIN$", "r", stdin);
	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1024);
}

void TellerEngineMain::setup()
{
	ShowConsole();
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

	// ContentsManagerのshareptrを引数に渡す
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
	mGame->PushScene(mScene);
	mScene->AddAgent(mAgent);

	//TODO:Delete
	auto animSeq = std::make_unique<AnimationSequencer>("args");

	auto mAnimator = std::make_unique<Circular>();
	auto textAgent = std::make_shared<MainTextArea>();
	mScene->AddAgent(textAgent);
	auto rpt = std::make_unique<Repeat>();

	auto cmcs = std::make_unique<ContentsManager<CSVLoader>>();
	auto kap = std::make_shared<Kappa>(fs::current_path()/ fs::path("kappa.png"));
	rpt->AttachToAgent(kap);

	mAnimator->AttachToAgent(mAgent);
	animSeq->AddAnimator(std::move(mAnimator));
	animSeq->AddAnimator(std::move(rpt));
	mCore->AddAnimSequencer(std::move(animSeq));
	mScene->AddAgent(kap);

	mCore->AppendEditor(fs::path(".csv"), std::make_unique<EpisodeEventEditor>());
	mCore->AppendEditor(std::make_unique<AssetViewer>());
	mCore->AppendEditor(std::make_unique<EpisodeEditor>());

	ci::app::setWindowSize(1280, 720);
	ci::app::setWindowPos(vec2(1920 / 2 - 1280 / 2, 1080 / 2 - 720 / 2));
}

void TellerEngineMain::createNewWindow()
{
	app::WindowRef newWindow = createWindow(Window::Format().size(400, 400));
	newWindow->setUserData(new WindowData);

	// for demonstration purposes, we'll connect a lambda unique to this window which fires on close
	int uniqueId = getNumWindows();
	newWindow->getSignalClose().connect(
		[uniqueId, this] { this->console() << "You closed window #" << uniqueId << std::endl; }
	);
}

void TellerEngineMain::update()
{
}

void TellerEngineMain::mouseDrag(MouseEvent event)
{
	WindowData* data = getWindow()->getUserData<WindowData>();
}

void TellerEngineMain::keyDown(KeyEvent event)
{
}

void TellerEngineMain::draw()
{
	gl::enableAlphaBlending();
	gl::clear(Color::black());

	/*
	処理記述ここから
	*/

	mCore->Tick();

	/*
	処理記述ここまで
	*/
}

// This line tells Cinder to actually create the application
CINDER_APP(TellerEngineMain, RendererGl)