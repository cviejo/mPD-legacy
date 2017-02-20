#include "App.h"
#include "PdGui.h"
#include "MainWindow.h"
#include "json.h"
#include "Svg.h"


//--------------------------------------------------------------
void App::setup(){

	_frame.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	// ofSetBackgroundAuto(false);
	ofSetWindowShape(ofGetWidth(), ofGetHeight());
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(20);
	ofBackground(125);

	GuiElement::Theme.load("themes/default.json");

	this->initSearchPaths();
	this->initAudio();
	this->initEventListeners();

	// debugging
	PdGui::instance().openPatch(ofToDataPath("patches/main-all.pd"));
	// // PdGui::instance().openPatch(ofToDataPath("patches/gatom-help.pd"));

	_mainWindow = (GuiElement*)new MainWindow();
	
#if defined(TARGET_OF_IOS)
	_pinch = [[ofPinchGestureRecognizer alloc] initWithView: ofxiPhoneGetGLView()];
#endif
}


//--------------------------------------------------------------
void App::initAudio(){

	auto outputChannels = 2;
	auto inputChannels  = 2;
	auto sampleRate     = 44100;
	auto ticksPerBuffer = 8;
	auto bufferSize     = PdGui::instance().blockSize() * ticksPerBuffer;

	ofSoundStreamSetup(outputChannels, inputChannels, this, sampleRate, bufferSize, 3);

	if (!PdGui::instance().init(outputChannels, inputChannels, sampleRate, ticksPerBuffer)){
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

#if defined(TARGET_ANDROID)
	ofAddListener(ofxAndroidEvents().scaleBegin, this, &App::onScaleBegin);
	ofAddListener(ofxAndroidEvents().scale,      this, &App::onScale);
	ofAddListener(ofxAndroidEvents().scaleEnd,   this, &App::onScaleEnd);
#elif !defined(TARGET_OF_IOS)
	ofAddListener(ofEvents().mouseScrolled,      this, &App::mouseScrolled);
#endif
}


//--------------------------------------------------------------
void App::draw(){


#if defined(TARGET_OF_IOS)
	if (_pinch->pinching){

		AppEvent event(AppEvent::TYPE_SCALE, "", 0, 0);

		event.value = _pinch->scale;

		ofNotifyEvent(AppEvent::events, event);
	}
#endif

	if (_mainWindow->updateNeeded()){

		_frame.begin();

		ofBackground(120);

		ofEnableAlphaBlending();

		_mainWindow->draw();

		ofDisableAlphaBlending();

		_frame.end();
	}

	ofSetColor(255, 255, 255, 255);

	_frame.draw(0, 0);
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

	if (!_computing){ return; }

	// TODO: if computing
	PdGui::instance().audioIn(input, bufferSize, nChannels);
}


//--------------------------------------------------------------
void App::audioRequested(float * output, int bufferSize, int nChannels) {

	if (!_computing){ return; }

	// TODO: if computing
	PdGui::instance().audioOut(output, bufferSize, nChannels);
}


//--------------------------------------------------------------
void App::touchDown(int aX, int aY, int aId){

	if (_scaling || aId){ return; } // pd can't handle multitouch anyway, afaik

	ofPoint loc(aX, aY);

	_mainWindow->touchDown(loc);
}


//--------------------------------------------------------------
void App::touchMoved(int aX, int aY, int aId){
		
	if (_scaling || aId){ return; }

	ofPoint loc(aX, aY);

	_mainWindow->touchMoved(loc);
}


//--------------------------------------------------------------
void App::touchUp(int aX, int aY, int aId){

	if (_scaling || aId){ return; }

	ofPoint loc(aX, aY);

	_mainWindow->touchUp(loc);
}


//--------------------------------------------------------------
void App::touchDoubleTap(int aX, int aY, int aId){

	if (_scaling || aId){ return; }

	ofPoint loc(aX, aY);

	AppEvent event(AppEvent::TYPE_KEY_PRESSED, (float)'e');

	ofNotifyEvent(AppEvent::events, event);
	// for (auto& elem : _guiElements){

		// if (elem->visible && elem->clickable && elem->inside(loc)){
			// elem->onDoubleClick(loc);
			// break;
		// }
	// }
}


//--------------------------------------------------------------
void App::touchCancelled(int x, int y, int id){ }


//--------------------------------------------------------------
void App::pause(){
	_computing = false;
}


//--------------------------------------------------------------
void App::stop(){
	_computing = false;
}


//--------------------------------------------------------------
void App::resume(){
	_computing = true;
}


//--------------------------------------------------------------
void App::reloadTextures(){ }


//--------------------------------------------------------------
bool App::backPressed(){ return false; }


//--------------------------------------------------------------
void App::okPressed(){ }


//--------------------------------------------------------------
void App::cancelPressed(){ }

#if defined(TARGET_ANDROID)
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
#elif !defined(TARGET_OF_IOS)

//--------------------------------------------------------------
void App::mouseScrolled(ofMouseEventArgs& mouse){

	AppEvent event(AppEvent::TYPE_SCALE, "", mouse.x, mouse.y);

	event.value = mouse.scrollY * 0.1f;

	ofNotifyEvent(AppEvent::events, event);
}
#endif

