// Derives window background color from three floating point args, e.g. orange: cmdLineArgs 1.0 0.5 0.25 

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class cmdLineArgsApp : public App {
 public:
	void setup();
	void draw();

	static void prepareSettings( App::Settings *settings ) { getArgs() = Platform::get()->getCommandLineArgs(); }
	static vector<string>& getArgs() { static vector<string> args; return args; }
};

void cmdLineArgsApp::setup()
{
	console() << "Arguments: " << std::endl;
	for( vector<string>::const_iterator argIt = getArgs().begin(); argIt != getArgs().end(); ++argIt )
		console() << " " << *argIt << std::endl;
}

void cmdLineArgsApp::draw()
{
	if( getArgs().size() >= 4 )
		gl::clear( Color( fromString<float>( getArgs()[1] ), fromString<float>( getArgs()[2] ), fromString<float>( getArgs()[3] ) ) );
	else
		gl::clear( Color( 0, 0, 0 ) );
}

// This line tells Cinder to actually create the application
CINDER_APP( cmdLineArgsApp, RendererGl, &cmdLineArgsApp::prepareSettings )
