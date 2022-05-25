#include"Core.h"
#include"Game.h"
#include"Scene.h"
#include"Agent.h"
#include"Asset.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace Teller;
namespace fs = std::filesystem;

// We'll create a new Cinder Application by deriving from the App class
class BasicAppMultiWindow : public App {
public:
	std::shared_ptr<TellerCore> mCore;
	std::shared_ptr<GameModule> mGame;
	std::shared_ptr<SceneModule> mScene;
	std::shared_ptr<Character> mCharacter;

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

	Color			mColor;
	list<vec2>		mPoints; // the points drawn into this window
};

void BasicAppMultiWindow::setup()
{
	// for the default window we need to provide an instance of WindowData
	getWindow()->setUserData(new WindowData);
	ImGui::Initialize();
	mCore = std::make_shared<TellerCore>();
	mGame = std::make_shared<GameModule>();
	mScene = std::make_shared<SceneModule>();
	
	mCharacter = std::make_shared<Character>(std::make_unique<Sprite>(std::string("kappa.png")));
	mCore->AddChildModule(mGame);
	mGame->AddChildModule(mScene);
	mScene->AddAgent(mCharacter);
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

	// add this point to the list
	data->mPoints.push_back(event.getPos());
}

void BasicAppMultiWindow::keyDown(KeyEvent event)
{
}

void BasicAppMultiWindow::draw()
{
	gl::clear(Color(0.1f, 0.1f, 0.15f));
	gl::enableAlphaBlending();

	float f=0.5;

	/*
	処理記述ここから
	*/

	mCore->Tick();

	/*
	処理記述ここまで
	*/
	gl::end();
}

// This line tells Cinder to actually create the application
CINDER_APP(BasicAppMultiWindow, RendererGl)