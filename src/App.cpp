#include "App.h"
#include "Globals.h"
#include "util/Env.h"
#include "gui/GUIElement.h"


//--------------------------------------------------------------
void App::setup(){

#ifdef TARGET_ANDROID
	ofAddListener(ofxAndroidEvents().scaleBegin, this, &App::onScaleBegin);
	ofAddListener(ofxAndroidEvents().scale,      this, &App::onScale);
	ofAddListener(ofxAndroidEvents().scaleEnd,   this, &App::onScaleEnd);
#else
	ofAddListener(ofEvents().mouseScrolled, this, &App::mouseScrolled);
#endif


	Globals::AppState = APP_STATE_START;

	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetWindowPosition(0, 20);
	ofSetFrameRate(30);

	this->init();
}


//--------------------------------------------------------------
void App::init(){

	Globals::DPI = Env::getDPI();

	Globals::Theme.load("themes/default.theme");
	Globals::Settings.load("settings.xml");

//	Font _font;
//	_font.load(Globals::Theme.node.font.height);
//
//	Globals::Pd.setFontSize(7, _font.lineHeight + 1);
//	Globals::Pd.setIOHeight(Globals::Theme.node.io.height);

	ofBackground(Globals::Globals::Theme.canvas.color.background);

	this->width  = ofGetWidth();
	this->height = ofGetHeight();

	this->initAudio();

	_canvas    = new Canvas();
	_frame     = new Frame();
	_objectTab = new ObjectTab();
	_menuTab   = new MenuTab();

	_objectTab->setContent(Globals::Pd.getObjectNames());

	Globals::Pd.addReceiver(*_canvas);

	Globals::AppState = APP_STATE_READY;
}


//--------------------------------------------------------------
void App::initAudio(){

	auto outputChannels = 2;
	auto inputChannels  = 2;
	auto sampleRate     = 44100;
	auto ticksPerBuffer = 8;

	ofSoundStreamSetup(outputChannels,
	                   inputChannels,
	                   this,
	                   sampleRate,
	                   Globals::Pd.getBlockSize() * ticksPerBuffer,
	                   3);

	if(!Globals::Pd.init(outputChannels, inputChannels, sampleRate, ticksPerBuffer)) {
		OF_EXIT_APP(1);
	}

	Globals::Pd.start();

	ofDirectory dir("extra");

	dir.listDir();

	for(int i = 0; i < dir.size(); i++){
		if (dir.getFile(i).isDirectory()){
			Globals::Pd.addToSearchPath(dir.getPath(i));
		}
	}

	 // Globals::Pd.openPatch("main6.pd");
	Globals::Pd.openPatch("main.pd");
	// Globals::Pd.subscribe("env");
	// Globals::Pd.addReceiver(*_canvas); // automatically receives from all subscribed sources
	// Globals::Pd.subscribe("toOF");
	// Globals::Pd.subscribe("env");

	// add message receiver, required if you want to recieve messages
	// Globals::Pd.addReceiver(*_canvas); // automatically receives from all subscribed sources
}


//--------------------------------------------------------------
void App::draw(){

	if (Globals::AppState == APP_STATE_START){ return; }

	ofEnableAlphaBlending();

	for (auto &elem : GUIElement::All){

		if(elem->visible){
			elem->draw();
		}
	}

	ofDisableAlphaBlending();
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(),2), 30, ofGetHeight()-90);  
}


//--------------------------------------------------------------
void App::keyPressed(int aKey){

	char key = (char)aKey;

	ofLog(OF_LOG_VERBOSE, "[key] " +  ofToString(key));

	if     (key == 't') { Globals::Theme.load("themes/default.theme"); }
	// else if(key == 'w') { _menuTab->state = _objectTab->state = COLLAPSE_STATE_EXPANDING; }
	// else if(key == 'q') { _menuTab->state = _objectTab->state = COLLAPSE_STATE_COLLAPSING; }
	//	else if(key == 'c') { _canvas->copy(); }
	//	else if(key == 'e') { _canvas->setEdit(!_canvas->getEdit()); }
	//	else if(key == 'r') { _canvas->reload(); }
	//	else if(key == 'x') { _canvas->cut(); }
	//	else if(key == 'u') { _canvas->undo(); }
	//	else if(key == 'p') { _canvas->paste(); }
	//	else if(key == 's') { _canvas->save(); }
	//	else if(key == '+') { _canvas->zoomIn(); }
	//	else if(key == '-') { _canvas->zoomOut(); }
	//	else if(key == 'd') { _canvas->createDummy(); }
	//	else if(key == 'n') { _canvas->clear(); }
}


//--------------------------------------------------------------
void App::gotMessage(ofMessage msg){
}




//--------------------------------------------------------------
void App::touchDown(int aX, int aY, int aId){

	if (aId){ return; }

	_down.set(aX, aY);

	for (auto it = GUIElement::All.rbegin(); it != GUIElement::All.rend(); ++it){
		
		GUIElement* elem = *it;

		if (elem->visible && elem->clickable && elem->inside(aX, aY)){

			elem->press.set(aX, aY);
			elem->pressed = true;
			elem->onPressed(aX, aY, aId);
			break;
		}
	}
}


//--------------------------------------------------------------
void App::touchMoved(int aX, int aY, int aId){

	if (aId){ return; }

	for (auto &elem : GUIElement::All){

		if(elem->pressed){
			elem->onDragged(aX, aY, aId);
		}
	}
}


//--------------------------------------------------------------
void App::touchUp(int aX, int aY, int aId){

	if (aId){ return; }

	_up.set(aX, aY);

	for (auto &elem : GUIElement::All){

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

	for (auto &elem : GUIElement::All){

		if (elem->visible && elem->clickable && elem->inside(aX, aY)){
			elem->onDoubleClick(aX, aY);
			break;
		}
	}
}


//--------------------------------------------------------------
void App::touchCancelled(int x, int y, int id){
}


//--------------------------------------------------------------
void App::reloadTextures(){
}


//--------------------------------------------------------------
bool App::backPressed(){

	return true;
}


//--------------------------------------------------------------
void App::okPressed(){
}


//--------------------------------------------------------------
void App::cancelPressed(){
}


//--------------------------------------------------------------
void App::audioReceived(float * input, int bufferSize, int nChannels) {

	if(Globals::AppState != APP_STATE_START){
		Globals::Pd.audioIn(input, bufferSize, nChannels);
	}
}


//--------------------------------------------------------------
void App::audioRequested(float * output, int bufferSize, int nChannels) {

	if(Globals::AppState != APP_STATE_START){
		Globals::Pd.audioOut(output, bufferSize, nChannels);
	}
}


//--------------------------------------------------------------
void App::osMessage(string aEvenType, string aMsg) {
}

#ifdef TARGET_ANDROID
//--------------------------------------------------------------
bool App::onScaleBegin(ofxAndroidScaleEventArgs& aArgs) {

	ofLogVerbose("[begin] log.");

	Globals::Pd.setCanvasMode(CANVAS_MODE_NONE);

	_canvas->scaling = true;

	return true;
}


//--------------------------------------------------------------
bool App::onScale(ofxAndroidScaleEventArgs& aArgs) {

	Globals::Settings.scale = Globals::Settings.scale * aArgs.getScaleFactor();

	ofLog(OF_LOG_VERBOSE, "[temp] focus:" + ofToString(aArgs.getFocusX()) + " " + ofToString(aArgs.getFocusY()));
	ofLog(OF_LOG_VERBOSE, "[temp] scale:" + ofToString(aArgs.getScaleFactor()));

	_canvas->setFocus(aArgs.getFocusX(), aArgs.getFocusY());

	return true;
}


//--------------------------------------------------------------
bool App::onScaleEnd(ofxAndroidScaleEventArgs& aArgs) {

	ofLog(OF_LOG_VERBOSE, "[end] log.");

	_canvas->scaling = false;

	return true;
}
#else

//--------------------------------------------------------------
void App::mouseScrolled(ofMouseEventArgs& mouse){

	ofLogVerbose("[scrollY] " + ofToString(mouse.scrollY));
	ofLogVerbose("[scrollX] " + ofToString(mouse.scrollX));

	Globals::Pd.setCanvasMode(CANVAS_MODE_NONE);

	_canvas->scaling = true;

	Globals::Settings.scale = Globals::Settings.scale + (mouse.scrollY * 0.1f);

	_canvas->setFocus(mouse.x, mouse.y);

	_canvas->scaling = false;

	// ofLog(OF_LOG_VERBOSE, "[temp] focus:" + ofToString(aArgs.getFocusX()) + " " + ofToString(aArgs.getFocusY()));
	// ofLog(OF_LOG_VERBOSE, "[temp] scale:" + ofToString(aArgs.getScaleFactor()));

	// _canvas->setFocus(aArgs.getFocusX(), aArgs.getFocusY());

	// _canvas->scaling = false;
}
#endif
