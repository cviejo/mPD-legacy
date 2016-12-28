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

	this->initAudio();
	this->initSearchPaths();
	this->initEventListeners();

	_guiElements.push_back(new Canvas());
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
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(),2), 30, ofGetHeight()-90);
}


//--------------------------------------------------------------
void App::keyPressed(int key){

	// debugging
	if      (key == 'a'){ }
	else if (key == 'c'){ PdGui::instance().canvasCopy(); }
	else if (key == 'p'){ PdGui::instance().canvasPaste(); }
	else if (key == 'u'){ PdGui::instance().canvasUndo(); }
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

	ofLogVerbose("[begin] log.");

	AppEvent event(AppEvent::TYPE_SCALE_BEGIN);
	ofNotifyEvent(AppEvent::events, event);
	// Globals::Pd.setCanvasMode(CANVAS_MODE_NONE);

	// _canvas->scaling = true;

	return true;
}


//--------------------------------------------------------------
bool App::onScale(ofxAndroidScaleEventArgs& aArgs) {

	AppEvent event(AppEvent::TYPE_SCALE, "", aArgs.getFocusX(), aArgs.getFocusY());

	event.value = aArgs.getScaleFactor();

	ofNotifyEvent(AppEvent::events, event);
	// Globals::Settings.scale = Globals::Settings.scale * aArgs.getScaleFactor();

	// ofLog(OF_LOG_VERBOSE, "[temp] focus:" + ofToString(aArgs.getFocusX()) + " " + ofToString(aArgs.getFocusY()));
	// ofLog(OF_LOG_VERBOSE, "[temp] scale:" + ofToString(aArgs.getScaleFactor()));

	// _canvas->setFocus(aArgs.getFocusX(), aArgs.getFocusY());

	return true;
}


//--------------------------------------------------------------
bool App::onScaleEnd(ofxAndroidScaleEventArgs& aArgs) {

	AppEvent event(AppEvent::TYPE_SCALE_END);
	ofNotifyEvent(AppEvent::events, event);
	// ofLog(OF_LOG_VERBOSE, "[end] log.");

	// _canvas->scaling = false;

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
	// Globals::Pd.setCanvasMode(CANVAS_MODE_NONE);

	// _canvas->scaling = true;

	// Globals::Settings.scale = Globals::Settings.scale + (mouse.scrollY * 0.1f);

	// _canvas->setFocus(mouse.x, mouse.y);

	// _canvas->scaling = false;

	// ofLog(OF_LOG_VERBOSE, "[temp] focus:" + ofToString(aArgs.getFocusX()) + " " + ofToString(aArgs.getFocusY()));
	// ofLog(OF_LOG_VERBOSE, "[temp] scale:" + ofToString(aArgs.getScaleFactor()));

	// _canvas->setFocus(aArgs.getFocusX(), aArgs.getFocusY());

	// _canvas->scaling = false;
}
#endif

