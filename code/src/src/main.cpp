#include "ofMain.h"
#include "testApp.h"

//========================================================================
int main( ){

	//ofSetupOpenGL(2304,1024, OF_WINDOW);
	ofSetupOpenGL(2048,768, OF_WINDOW);
	// pass in width and height too:

		// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:

	ofRunApp(new testApp());

}
