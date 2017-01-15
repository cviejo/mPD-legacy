#include "App.h"
#include "PdGui.h"
#include "Canvas.h"
#include "Button.h"


bool computing = true;

//--------------------------------------------------------------
void App::setup(){

	// ofSetBackgroundAuto(false);
	// ofBackground(0);
	ofBackground(255);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(21);
	ofSetWindowPosition(152,1094);

	this->initSearchPaths();
	this->initAudio();
	this->initEventListeners();

	// Button* btn;

	// btn = new Button("edit");
	// btn->setPosition(0, 0);
	// _guiElements.push_back(btn);

	// btn = new Button("copy");
	// btn->setPosition(0, 100);
	// _guiElements.push_back(btn);

	// btn = new Button("paste");
	// btn->setPosition(0, 200);
	// _guiElements.push_back(btn);

	// btn = new Button("undo");
	// btn->setPosition(0, 300);
	// _guiElements.push_back(btn);

	Canvas* cnv = new Canvas();

	_guiElements.push_back(cnv);

	// debugging
	// PdGui::instance().openPatch(ofToDataPath("basic.pd"));
	PdGui::instance().openPatch(ofToDataPath("main.pd"));
	PdGui::instance().openPatch(ofToDataPath("gatom-help.pd"));

	cnv->set(PdGui::instance().getCanvases()[0]);
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
}


//--------------------------------------------------------------
void App::initSearchPaths(){

	ofDirectory extrasFolder("extra");

	extrasFolder.listDir();

	for(int i = 0; i < extrasFolder.size(); i++){

		if (extrasFolder.getFile(i).isDirectory()){
			string path = ofFilePath::getAbsolutePath(ofToDataPath(extrasFolder.getPath(i)));
			PdGui::instance().addToSearchPath(path);
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

	for (auto i = _guiElements.rbegin(); i != _guiElements.rend(); ++i){

		GuiElement* elem = *i;

		if (elem->visible){
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

	AppEvent event(AppEvent::TYPE_KEY_PRESSED, (float)key);

	ofNotifyEvent(AppEvent::events, event);
}



//--------------------------------------------------------------
void App::windowResized(int w, int h){ }


//--------------------------------------------------------------
void App::gotMessage(ofMessage msg){ }


//--------------------------------------------------------------
void App::audioReceived(float * input, int bufferSize, int nChannels) {

	if (!computing){ return; }

	// TODO: if computing
	PdGui::instance().audioIn(input, bufferSize, nChannels);
}


//--------------------------------------------------------------
void App::audioRequested(float * output, int bufferSize, int nChannels) {

	if (!computing){ return; }

	// TODO: if computing
	PdGui::instance().audioOut(output, bufferSize, nChannels);
}


//--------------------------------------------------------------
void App::touchDown(int aX, int aY, int aId){

	if (_scaling || aId){ return; } // pd can't handle multitouch anyway, afaik

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

	if (_scaling || aId){ return; }

	for (auto& elem : _guiElements){

		if (elem->pressed){
			elem->onDragged(aX, aY, aId);
		}
	}
}


//--------------------------------------------------------------
void App::touchUp(int aX, int aY, int aId){

	if (_scaling || aId){ return; }

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
void App::touchDoubleTap(int aX, int aY, int aId){

	if (_scaling || aId){ return; }

	for (auto& elem : _guiElements){

		if (elem->visible && elem->clickable && elem->inside(aX, aY)){
			elem->onDoubleClick(aX, aY);
			break;
		}
	}
}


//--------------------------------------------------------------
void App::touchCancelled(int x, int y, int id){ }


//--------------------------------------------------------------
void App::pause(){
	computing = false;
}


//--------------------------------------------------------------
void App::stop(){
	computing = false;
}


//--------------------------------------------------------------
void App::resume(){
	computing = true;
}


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

	_scaling = true;

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

	_scaling = false;

	AppEvent event(AppEvent::TYPE_SCALE_END);

	ofNotifyEvent(AppEvent::events, event);

	return true;
}
#else

//--------------------------------------------------------------
void App::mouseScrolled(ofMouseEventArgs& mouse){

	// ofLogVerbose("[scrollY] " + ofToString(mouse.scrollY));
	// ofLogVerbose("[scrollX] " + ofToString(mouse.scrollX));

	AppEvent event(AppEvent::TYPE_SCALE, "", mouse.x, mouse.y);

	event.value = mouse.scrollY * 0.1f;

	ofNotifyEvent(AppEvent::events, event);
}
#endif

