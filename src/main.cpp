#include "ofMain.h"
#include "ofApp.h"
#include "ofAppNoWindow.h"

//========================================================================
int main( ){


    //Could headless be the right way to do it? This was crashing
    //at the OFImage init in the setup()
//    ofAppNoWindow w;
//    ofSetupOpenGL(&w,1024,768, OF_WINDOW);
    
    ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
