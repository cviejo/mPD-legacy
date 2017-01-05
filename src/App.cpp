#include "App.h"
#include "PdGui.h"
#include "Canvas.h"


//--------------------------------------------------------------
void App::setup(){

	// ofSetBackgroundAuto(false);
	// ofBackground(0);
	ofBackground(255);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(21);
	ofSetWindowPosition(152,1094);

	this->initAudio();
	this->initSearchPaths();
	this->initEventListeners();

	_guiElements.push_back(new Canvas());

	((Canvas*)_guiElements[0])->set(PdGui::instance().getCanvases()[0]);
}


//--------------------------------------------------------------
void App::initAudio(){

	auto outputChannels = 2;
	auto inputChannels  = 2;
	auto sampleRate     = 44100;
	auto ticksPerBuffer = 8;
	auto bufferSize     = PdGui::instance().blockSize() * ticksPerBuffer;

	ofSoundStreamSetup(outputChannels, inputChannels, this, sampleRate, bufferSize, 3);

	if(!PdGui::instance().init(outputChannels, inputChannels, sampleRate, ticksPerBuffer)) {
		return;
	}

	PdGui::instance().startAudio();
	PdGui::instance().openPatch(ofToDataPath("main.pd"));
	
}


//--------------------------------------------------------------
void App::initSearchPaths(){

	ofDirectory extrasFolder("extra");

	extrasFolder.listDir();

	for(int i = 0; i < extrasFolder.size(); i++){

		if (extrasFolder.getFile(i).isDirectory()){
			PdGui::instance().addToSearchPath(extrasFolder.getPath(i));
		}
	}
}


//--------------------------------------------------------------
void App::initEventListeners(){

#ifdef TARGET_ANDROID
	ofAddListener(ofxAndroidEvents().scaleBegin, this, &App::onScaleBegin);
	ofAddListener(ofxAndroidEvents().scale,      this, &App::onScale);
	ofAddListener(ofxAndroidEvents().scaleEnd,   this, &App::onScaleEnd);
#else
	ofAddListener(ofEvents().mouseScrolled,      this, &App::mouseScrolled);
#endif
}


//--------------------------------------------------------------
void App::draw(){

	// if (Globals::AppState == APP_STATE_START){ return; }

	ofEnableAlphaBlending();

	for (auto& elem : _guiElements){

		if(elem->visible){
			elem->draw();
		}
	}

	ofDisableAlphaBlending();

	// ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(),2), 30, ofGetHeight()-90);
	// ofDrawBitmapString("x:   " + ofToString(ofGetWindowPositionX()), 30, 30);
	// ofDrawBitmapString("y:   " + ofToString(ofGetWindowPositionY()), 30, 50);
}


//--------------------------------------------------------------
void App::keyPressed(int key){

	auto canvases = PdGui::instance().getCanvases();

	if (!canvases.size()){ return; }

	string cmd = "";

	// debugging
	if      (key == 'a'){ cmd = canvases[0]->id + " selectall"; }
	else if (key == 'c'){ cmd = canvases[0]->id + " copy"; }
	else if (key == 'e'){ cmd = canvases[0]->id + " selectall"; }
	else if (key == 'p'){ cmd = canvases[0]->id + " paste"; }
	else if (key == 'u'){ cmd = canvases[0]->id + " undo"; }

	if (cmd != ""){
		PdGui::instance().pdsend(cmd);
	}
}


//--------------------------------------------------------------
void App::windowResized(int w, int h){ }


//--------------------------------------------------------------
void App::gotMessage(ofMessage msg){ }


//--------------------------------------------------------------
void App::audioReceived(float * input, int bufferSize, int nChannels) {

	// TODO: if computing
	PdGui::instance().audioIn(input, bufferSize, nChannels);
}


//--------------------------------------------------------------
void App::audioRequested(float * output, int bufferSize, int nChannels) {

	// TODO: if computing
	PdGui::instance().audioOut(output, bufferSize, nChannels);
}


//--------------------------------------------------------------
void App::touchDown(int aX, int aY, int aId){

	if (aId){ return; } // pd can't handle multitouch anyway

	for (auto& elem : _guiElements){

		if (elem->visible && elem->clickable && elem->inside(aX, aY)){

			elem->pressed = true;
			elem->pressedPoint.set(aX, aY);
			elem->onPressed(aX, aY, aId);
			break;
		}
	}
}


//--------------------------------------------------------------
void App::touchMoved(int aX, int aY, int aId){

	if (aId){ return; }

	for (auto& elem : _guiElements){

		if (elem->pressed){
			elem->onDragged(aX, aY, aId);
		}
	}
}


//--------------------------------------------------------------
void App::touchUp(int aX, int aY, int aId){

	if (aId){ return; }

	for (auto& elem : _guiElements){

		if (elem->pressed){

			if (elem->inside(aX, aY)){
				elem->onClick();
			}

			elem->onReleased(aX, aY, aId);
			elem->pressed = false;
		}
	}
}


//--------------------------------------------------------------
void App::touchDoubleTap(int aX, int aY, int aId){ }


//--------------------------------------------------------------
void App::touchCancelled(int x, int y, int id){ }


//--------------------------------------------------------------
void App::pause(){ }


//--------------------------------------------------------------
void App::stop(){ }


//--------------------------------------------------------------
void App::resume(){ }


//--------------------------------------------------------------
void App::reloadTextures(){ }


//--------------------------------------------------------------
bool App::backPressed(){ return false; }


//--------------------------------------------------------------
void App::okPressed(){ }


//--------------------------------------------------------------
void App::cancelPressed(){ }

#ifdef TARGET_ANDROID
//--------------------------------------------------------------
void App::swipe(ofxAndroidSwipeDir swipeDir, int id){ }


//--------------------------------------------------------------
bool App::onScaleBegin(ofxAndroidScaleEventArgs& aArgs) {

	AppEvent event(AppEvent::TYPE_SCALE_BEGIN);
	ofNotifyEvent(AppEvent::events, event);

	return true;
}


//--------------------------------------------------------------
bool App::onScale(ofxAndroidScaleEventArgs& aArgs) {

	AppEvent event(AppEvent::TYPE_SCALE, "", aArgs.getFocusX(), aArgs.getFocusY());

	event.value = aArgs.getScaleFactor();

	ofNotifyEvent(AppEvent::events, event);

	return true;
}


//--------------------------------------------------------------
bool App::onScaleEnd(ofxAndroidScaleEventArgs& aArgs) {

	AppEvent event(AppEvent::TYPE_SCALE_END);
	ofNotifyEvent(AppEvent::events, event);

	return true;
}
#else

//--------------------------------------------------------------
void App::mouseScrolled(ofMouseEventArgs& mouse){

	ofLogVerbose("[scrollY] " + ofToString(mouse.scrollY));
	ofLogVerbose("[scrollX] " + ofToString(mouse.scrollX));

	AppEvent event(AppEvent::TYPE_SCALE, "", mouse.x, mouse.y);

	event.value = mouse.scrollY * 0.1f;

	ofNotifyEvent(AppEvent::events, event);
}
#endif

