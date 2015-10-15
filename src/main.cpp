#include "ofMain.h"
#include "Core.h"

Core * core;

//========================================================================
int main( ){

	ofSetupOpenGL(1280,800, OF_WINDOW);			// OF_FULLSCREEN
    
    core = (Core*)(new Core());
    
	ofRunApp( core);

}
