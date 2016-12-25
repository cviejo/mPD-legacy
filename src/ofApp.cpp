#include "ofApp.h"
#include "PdGui.h"


//--------------------------------------------------------------
void ofApp::setup(){

	// ofSetBackgroundAuto(false);
	ofBackground(124);
	ofSetColor(255,255,255);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(21);

	this->initAudio();
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
void ofApp::update(){ }


//--------------------------------------------------------------
void ofApp::draw(){

	ofScale(2, 2);

	vector<PdCanvas*> canvases = PdGui::instance().getCanvases();

	ofSetColor(255);

	for (auto cnv : canvases){

		for (auto node : cnv->nodes){
			ofSetColor(0);
			ofDrawRectangle(node->x, node->y, node->width, node->height);
			if (node->selected){
				ofSetColor(255,0,0);
			}
			else {
				ofSetHexColor(node->backgroundColor);
			}
			ofDrawRectangle(node->x + 1, node->y + 1, node->width - 2, node->height - 2);
		}

		for (auto node : cnv->connections){
			ofSetColor(0);
			ofDrawLine(node->x1, node->y1, node->x2, node->y2);
		}

		if (cnv->mode == PdCanvas::MODE_REGION){
			ofSetColor(0, 120);
			ofDrawRectangle(cnv->region);
		}
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){ }


//--------------------------------------------------------------
void ofApp::keyReleased(int key){ }


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){ }


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	PdGui::instance().canvasDragged(x/2, y/2);
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	PdGui::instance().canvasPressed(x/2, y/2);
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	PdGui::instance().canvasReleased(x/2, y/2);
}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){ }


//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){ }


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){ }


//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){ }


//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ }


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
void ofApp::touchDown(int x, int y, int id){ }


//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){ }


//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){ }


//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){ }


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
