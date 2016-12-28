#include "Canvas.h"


//--------------------------------------------------------------
Canvas::Canvas(){

	_viewPort.setPosition(0, 0);
	_scale = 1.f;

	this->x      = 0;
	this->y      = 0;
	this->width  = ofGetWidth();
	this->height = ofGetHeight();
	// _renderer = new CanvasRenderer();
	// // _renderer.font = _font;

	// //todo: tidy up here
	// _font.load(50);
	// // _font.load(Globals::Theme.node.font.height);
	// _consoleFont.load(Globals::Theme.console.font.height);

	// _renderer->font = _font;

	// float lineHeight = _font.lineHeight / 50.0f * Globals::Theme.node.font.height;

	// ofLog(OF_LOG_VERBOSE, "[lin] " +  ofToString(lineHeight) + ".");

	// Globals::Pd.setFontSize(7, lineHeight);
	// // Globals::Pd.setFontSize(6, 8);
	// Globals::Pd.setIOHeight(Globals::Theme.node.io.height);

	// this->x      = Globals::Theme.tab.collapsedWidth;
	// this->y      = Globals::Theme.button.height;
	// this->width  = ofGetWidth();
	// this->height = ofGetHeight() - Globals::Theme.button.height * 2;

	// this->initGrid();
}

//---------------------------VIRTUAL--------------------------//
//--------------------------------------------------------------
void Canvas::draw(){

	ofPushMatrix();
	ofTranslate(this->x, this->y);
	ofScale(_scale, _scale);

	vector<PdCanvas*> canvases = PdGui::instance().getCanvases();

	ofSetColor(255);

	for (auto& canvas : canvases){

		for (auto node : canvas->nodes){
			ofSetColor(0);
			// ofDrawRectangle(node->x, node->y, node->width, node->height);
			ofDrawRectangle(*node);
			if (node->selected){
				ofSetColor(255,0,0);
			}
			else {
				ofSetHexColor(node->backgroundColor);
			}
			ofDrawRectangle(node->x + 1, node->y + 1, node->width - 2, node->height - 2);

			for (auto inlet : node->inlets){
				ofSetColor(255,255,0);
				ofDrawRectangle(inlet);
			}
			for (auto outlet : node->outlets){
				ofSetColor(255,255,0);
				ofDrawRectangle(outlet);
			}
		}

		for (auto node : canvas->connections){
			ofSetColor(0);
			ofDrawLine(node->x1, node->y1, node->x2, node->y2);
		}

		if (canvas->mode == PdCanvas::MODE_REGION){
			ofSetColor(0, 120);
			ofDrawRectangle(canvas->region);
		}
	}

	ofPopMatrix();
}


//--------------------------------------------------------------
void Canvas::drawRegion(PdCanvas* aCanvas){

	if (aCanvas->mode == PdCanvas::MODE_REGION){

		ofSetColor(100);
		ofNoFill();
		ofDrawRectangle(aCanvas->region);

		ofSetColor(100, 100);
		ofFill();
		ofDrawRectangle(aCanvas->region);
	}
}


//--------------------------------------------------------------
void Canvas::onPressed(int aX, int aY, int aId){

	ofPoint p = this->transformToPdCoordinates(aX, aY);

	PdGui::instance().canvasPressed(p.x, p.y);

	// if (!scaling){

		// _pressLoc.set(this->transformLoc(aX, aY, TRANSFORM_MPD_TO_PD));
		// _mouseLoc.set(_pressLoc);

		// Globals::Pd.canvasPressed(_mouseLoc.x, _mouseLoc.y);
	// }
}


//--------------------------------------------------------------
void Canvas::onDragged(int aX, int aY, int aId){

	ofPoint p = this->transformToPdCoordinates(aX, aY);

	PdGui::instance().canvasDragged(p.x, p.y);
	// _mouseLoc.set(this->transformLoc(aX, aY, TRANSFORM_MPD_TO_PD));

	// CanvasMode mode = Globals::Pd.getCanvasMode();

	// if (mode != CANVAS_MODE_CONNECT){

		// if (scaling || (!Globals::Pd.getCanvasEditMode() && !Globals::Pd.getNodePressed() && mode == CANVAS_MODE_NONE)){
		// // if (!Globals::Pd.getCanvasEditMode() && mode == CANVAS_MODE_NONE){

			// _draggedLoc.x = _mouseLoc.x - _pressLoc.x;
			// _draggedLoc.y = _mouseLoc.y - _pressLoc.y;

			// this->clipOffset();
		// }
		// else {

			// Globals::Pd.canvasDragged(_mouseLoc.x, _mouseLoc.y);
		// }
	// }
}


//--------------------------------------------------------------
void Canvas::onReleased(int aX, int aY, int aId){

	ofPoint p = this->transformToPdCoordinates(aX, aY);

	PdGui::instance().canvasReleased(p.x, p.y);
	// _mouseLoc.set(this->transformLoc(aX, aY, TRANSFORM_MPD_TO_PD));

	// _offsetLoc.x += _draggedLoc.x;
	// _offsetLoc.y += _draggedLoc.y;

	// _pressLoc  .set(-1, -1);
	// _draggedLoc.set(0, 0);

	// if (!scaling){
		// Globals::Pd.canvasReleased(_mouseLoc.x, _mouseLoc.y);
	// }
}


//--------------------------------------------------------------
void Canvas::onDoubleClick(int aX, int aY){

	// if (!Globals::Pd.getNodePressed()){
		// auto editMode = Globals::Pd.getCanvasEditMode();
		// Globals::Pd.setCanvasEditMode(!editMode);
	// }
}


//--------------------------------------------------------------
void Canvas::onPressCancel(){
}


//--------------------------------------------------------------
void Canvas::onAppEvent(AppEvent& aAppEvent){

	ofLogVerbose("event") << "hm";
	if (aAppEvent.type == AppEvent::TYPE_SCALE){
#ifdef TARGET_ANDROID
		_scale *= aAppEvent.value;
#else
		_scale += aAppEvent.value;
#endif
	}

	// if (aAppEvent.type == AppEvent::TYPE_BUTTON_PRESSED){

		// if (aAppEvent.message == "edit-button"){
			// Globals::Pd.setCanvasEditMode(aAppEvent.value);
		// }
		// else if (aAppEvent.message == "grid-button"){
			// Globals::Settings.gridActive = aAppEvent.value;
			// Globals::Pd.setCanvasGridMode(aAppEvent.value);
		// }
		// else if (aAppEvent.message == "settings-button"){
		// }
		// else if (aAppEvent.message == "trash-button"){
			// Globals::Pd.canvasDelete();
		// }
		// else if (aAppEvent.message == "copy-button"){
			// Globals::Pd.canvasCopy();
		// }
		// else if (aAppEvent.message == "paste-button"){
			// Globals::Pd.canvasPaste();
		// }
		// else if (aAppEvent.message == "undo-button"){
			// Globals::Pd.canvasUndo();
		// }
	// }
	// else if(aAppEvent.type == AppEvent::TYPE_CREATE_OBJECT){

		// PdNode node;

		// ofPoint loc = this->transformLoc(aAppEvent.x, aAppEvent.y, TRANSFORM_MPD_TO_PD);

		// node.className = aAppEvent.message;
		// node.x         = loc.x;
		// node.y         = loc.y;

		// Globals::Pd.canvasCreateObject(node);
	// }
}


//--------------------------------------------------------------
ofPoint Canvas::transformToPdCoordinates(float aX, float aY){

	ofPoint result;

	result.x = (aX - this->x) / _scale - _viewPort.x; // Globals::Settings.scale;
	result.y = (aY - this->y) / _scale - _viewPort.y; // Globals::Settings.scale;

	return result;
}

