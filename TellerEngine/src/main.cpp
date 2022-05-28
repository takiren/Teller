#include <io.h>
#include <Fcntl.h>
#include<Windows.h>
#include <list>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include"cinder/ImageIo.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include"cinder/Log.h"
#include "cinder/CinderImGui.h"

#include"TellerCore.h"
#include"Game.h"
#include"Scene.h"
#include"Agent.h"
#include"Animation.h"


using namespace ci;
using namespace ci::app;
using namespace std;
using namespace Teller;
namespace fs = std::filesystem;

// We'll create a new Cinder Application by deriving from the App class
class BasicAppMultiWindow : public App {
public:
	std::shared_ptr<TellerCore> mCore;

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

	auto mAnimator = std::make_unique<Circular>();

	/*mCore->AttachDeltaTimeMessanger(0,
		[&](float deltaTime) { mAnimator->SetDeltaTime(deltaTime); }
	);*/

	auto mGame = std::make_shared<GameModule>();
	auto mScene = std::make_shared<SceneModule>();
	auto mAgent = std::make_shared<RectAgent>();

	mCore->AddModule(mGame);
	mGame->AddChildModule(mScene);
	mScene->AddAgent(mAgent);

	auto animSeq = std::make_unique<AnimationSequencer>();

	mAnimator->AttachToAgent(mAgent);
	animSeq->AddAnimator(std::move(mAnimator));
	mCore->AddAnimSequencer(std::move(animSeq));
	/*mAnimator->Attach(0,
		[](vec2& _pos, vec2& _rot, vec2& _scale) {
			mAgent->Animate(_pos, _rot, _scale);
		});*/

		//��Âɍl������|�C���^�ǂ��������Ă邩��폜�B
		/*setvbuf(stdout, NULL, _IONBF, 0);

		AllocConsole();
		auto hConsole = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
		*stdout = *_fdopen(hConsole, "w");*/

	ci::app::setWindowSize(1280, 720);
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

	/*
	�����L�q��������
	*/
	mCore->Tick();
	/*
	�����L�q�����܂�
	*/
	gl::end();
}

// This line tells Cinder to actually create the application
CINDER_APP(BasicAppMultiWindow, RendererGl)