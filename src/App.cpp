#include "ofApp.h"
#include "PdGui.h"
#include "Canvas.h"


//--------------------------------------------------------------
void ofApp::setup(){

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
void ofApp::initAudio(){

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
void ofApp::initSearchPaths(){

	ofDirectory extrasFolder("extra");

	extrasFolder.listDir();

	for(int i = 0; i < extrasFolder.size(); i++){

		if (extrasFolder.getFile(i).isDirectory()){
			PdGui::instance().addToSearchPath(extrasFolder.getPath(i));
		}
	}
}


//--------------------------------------------------------------
void ofApp::initEventListeners(){

#ifdef TARGET_ANDROID
	ofAddListener(ofxAndroidEvents().scaleBegin, this, &ofApp::onScaleBegin);
	ofAddListener(ofxAndroidEvents().scale,      this, &ofApp::onScale);
	ofAddListener(ofxAndroidEvents().scaleEnd,   this, &ofApp::onScaleEnd);
#else
	ofAddListener(ofEvents().mouseScrolled,      this, &ofApp::mouseScrolled);
#endif
}


//--------------------------------------------------------------
void ofApp::draw(){

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
void ofApp::keyPressed(int key){

	// debugging
	if      (key == 'a'){ }
	else if (key == 'c'){ PdGui::instance().canvasCopy(); }
	else if (key == 'p'){ PdGui::instance().canvasPaste(); }
	else if (key == 'u'){ PdGui::instance().canvasUndo(); }
}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){ }


//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){ }


//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels) {

	// TODO: if computing
	PdGui::instance().audioIn(input, bufferSize, nChannels);
}


//--------------------------------------------------------------
void ofApp::audioRequested(float * output, int bufferSize, int nChannels) {

	// TODO: if computing
	PdGui::instance().audioOut(output, bufferSize, nChannels);
}


//--------------------------------------------------------------
void ofApp::touchDown(int aX, int aY, int aId){

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
void ofApp::touchMoved(int aX, int aY, int aId){

	if (aId){ return; }

	for (auto& elem : _guiElements){

		if (elem->pressed){
			elem->onDragged(aX, aY, aId);
		}
	}
}


//--------------------------------------------------------------
void ofApp::touchUp(int aX, int aY, int aId){

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
void ofApp::touchDoubleTap(int aX, int aY, int aId){ }


//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){ }


//--------------------------------------------------------------
void ofApp::pause(){ }


//--------------------------------------------------------------
void ofApp::stop(){ }


//--------------------------------------------------------------
void ofApp::resume(){ }


//--------------------------------------------------------------
void ofApp::reloadTextures(){ }


//--------------------------------------------------------------
bool ofApp::backPressed(){ return false; }


//--------------------------------------------------------------
void ofApp::okPressed(){ }


//--------------------------------------------------------------
void ofApp::cancelPressed(){ }

#ifdef TARGET_ANDROID
//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){ }


//--------------------------------------------------------------
bool ofApp::onScaleBegin(ofxAndroidScaleEventArgs& aArgs) {

	ofLogVerbose("[begin] log.");

	// Globals::Pd.setCanvasMode(CANVAS_MODE_NONE);

	// _canvas->scaling = true;

	return true;
}


//--------------------------------------------------------------
bool ofApp::onScale(ofxAndroidScaleEventArgs& aArgs) {

	// Globals::Settings.scale = Globals::Settings.scale * aArgs.getScaleFactor();

	// ofLog(OF_LOG_VERBOSE, "[temp] focus:" + ofToString(aArgs.getFocusX()) + " " + ofToString(aArgs.getFocusY()));
	// ofLog(OF_LOG_VERBOSE, "[temp] scale:" + ofToString(aArgs.getScaleFactor()));

	// _canvas->setFocus(aArgs.getFocusX(), aArgs.getFocusY());

	return true;
}


//--------------------------------------------------------------
bool ofApp::onScaleEnd(ofxAndroidScaleEventArgs& aArgs) {

	// ofLog(OF_LOG_VERBOSE, "[end] log.");

	// _canvas->scaling = false;

	return true;
}
#else

//--------------------------------------------------------------
void ofApp::mouseScrolled(ofMouseEventArgs& mouse){

	ofLogVerbose("[scrollY] " + ofToString(mouse.scrollY));
	ofLogVerbose("[scrollX] " + ofToString(mouse.scrollX));

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
