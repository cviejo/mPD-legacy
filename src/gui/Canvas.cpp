#include "Canvas.h"


int gridStepSize = 20; // Globals::Theme.grid.cell.width 


//--------------------------------------------------------------
Canvas::Canvas(){

	this->x      = 0;
	this->y      = 0;
	this->width  = ofGetWidth();
	this->height = ofGetHeight();

	_font.load("fonts/DejaVuSansMono.ttf", 70, true, true);
	// _font.load("fonts/VeraMono.ttf",       70, true, true);
	// _font.load("fonts/DroidSansMono.ttf",  70, true, true);
	// _font.load("fonts/Inconsolata.otf",    70, true, true);
	// _font.load("fonts/UbuntuMono-R.ttf",   70, true, true);
	_font.setLineHeight(100.0f);
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

	this->initGrid();

	// PdGui::instance().canvasPressed(_current, p.x, p.y);
}


//--------------------------------------------------------------
void Canvas::initGrid(){

	int gridWidth  = this->width  + gridStepSize;// * 3;
	int gridHeight = this->height + gridStepSize;// * 3;

	_grid.allocate(gridWidth, gridHeight, GL_RGBA);
	_grid.begin();

	ofClear(255, 0);
	// ofBackground(Globals::Theme.canvas.color.background);
	// ofSetColor(Globals::Theme.canvas.color.background);
	// ofSetColor(0);
	// ofBackground(0,0,255);
	ofSetColor(255);
	ofDrawRectangle(0, 0, gridWidth, gridHeight);
	// ofSetColor(Globals::Theme.grid.color.front);

	ofSetColor(200);
	for(int i = 0; i < gridWidth; i += gridStepSize){
		for(int j = 0; j < gridHeight; j += gridStepSize){
			ofDrawCircle(i, j, 1);
		}
	}

	_grid.end();

	// Globals::Pd.setCanvasGridMode(true);
	// Globals::Pd.setCanvasGridSize(Globals::Theme.grid.cell.width, 
											// Globals::Theme.grid.cell.height);
}


//--------------------------------------------------------------
void Canvas::set(PdCanvas* aCanvas){
	_current = aCanvas;
}


//---------------------------VIRTUAL--------------------------//
//--------------------------------------------------------------
void Canvas::draw(){

	if (!_current){ return; }

	ofPushMatrix();

	ofTranslate(this->x, this->y);
	ofScale(_current->scale, _current->scale);

	this->drawGrid();

	ofTranslate(_current->viewPort.getPosition() * -1);

	this->drawNodes();
	this->drawConnections();
	this->drawRegion();

	if (_current->mode == PdCanvas::MODE_CONNECT){

		// ofPoint p2(ofGetMouseX(), ofGetMouseY());
		ofPoint p2 = this->transformToPdCoordinates(ofGetMouseX(), ofGetMouseY());

		ofDrawLine(_connectionStart->getCenter(), p2);
	}
	

	ofPopMatrix();
}


//--------------------------------------------------------------
void Canvas::drawGrid(){

	if (_current->scale >= 1){ // TODO: && grid active

		ofSetColor(255);

		_grid.draw((int)(-_current->viewPort.x) % gridStepSize,
		           (int)(-_current->viewPort.y) % gridStepSize);
	}
}


//--------------------------------------------------------------
void Canvas::drawNodes(){

	for (auto node : _current->nodes){

		if (_current->viewPort.intersects(*node)){

			this->drawNodeBackground(node);
			this->drawNodeText(node);

			if (node->type == "iemgui"){

				PdIemGui* guiNode = (PdIemGui*)node;

				if (guiNode->canvas){
					ofFill();
					ofSetHexColor(guiNode->backgroundColor);
					ofDrawRectangle(*(guiNode->canvas));
				}
				if (guiNode->label){
					this->drawNodeText(guiNode->label);
				}

				if (guiNode->iemType == "slider"){
					ofDrawRectangle(guiNode->slider);
				}

				if (guiNode->iemType == "bng"){

					ofSetHexColor(guiNode->foregroundColor);
					ofFill();
					ofDrawCircle(guiNode->getCenter(), (guiNode->width - 2) / 2);

					ofSetColor(0); // TODO: borderColor
					ofSetLineWidth(_current->scale);
					ofNoFill();
					ofDrawCircle(guiNode->getCenter(), (guiNode->width - 2) / 2);
				}
				else if (guiNode->iemType == "toggle" && guiNode->value){

					ofSetHexColor(guiNode->foregroundColor);
					ofSetLineWidth(_current->scale);

					auto pad    = 2;
					auto top    = guiNode->y + pad;
					auto left   = guiNode->x + pad;
					auto bottom = guiNode->getBottom() - pad;
					auto right  = guiNode->getRight()  - pad;

					ofDrawLine(left, top,    right, bottom);
					ofDrawLine(left, bottom, right, top);
				}
			}

			for (auto inlet : node->inlets){
				this->drawNodeIo(inlet);
			}

			for (auto outlet : node->outlets){
				this->drawNodeIo(outlet);
			}
		}
	}
}


//--------------------------------------------------------------
void Canvas::drawConnections(){

	for (auto conn : _current->connections){

		if (_current->viewPort.inside(conn->getPosition()) || _current->viewPort.inside(conn->x2, conn->y2)){
			ofSetColor(119);
			ofSetLineWidth(_current->scale);
			ofDrawLine(conn->x, conn->y, conn->x2, conn->y2);
		}
	}
}


//--------------------------------------------------------------
void Canvas::drawRegion(){

	if (_current->mode == PdCanvas::MODE_REGION){

		ofSetColor(100);
		ofNoFill();
		ofDrawRectangle(_current->region);

		ofSetColor(100, 100);
		ofFill();
		ofDrawRectangle(_current->region);
	}
}


//--------------------------------------------------------------
void Canvas::drawNodeBackground(PdNode* aNode){

	ofFill();

	int top    = aNode->getTop();
	int left   = aNode->getLeft();
	int right  = aNode->getRight();
	int bottom = aNode->getBottom();

	int borderCol       = 204;
	int numboxBorderCol = 168;
	int backCol         = 240;

	if (aNode->selected){
		ofSetColor(140);
		ofDrawRectangle(aNode->x - 2, aNode->y - 2, aNode->width + 4, aNode->height + 4);
	}

	if (aNode->type == "msg"){
		// ofSetColor(Globals::Theme.node.color.border);
		ofSetColor(borderCol);
		ofBeginShape();
			ofVertex(left,      top);
			ofVertex(right + 4, top);
			ofVertex(right,     top + 4);
			ofVertex(right,     bottom - 4);
			ofVertex(right + 4, bottom);
			ofVertex(left,      bottom);
		ofEndShape();

		// ofSetColor(255);
		ofSetColor(248, 248, 246);
		ofBeginShape();
			ofVertex(left  + 1, top + 1);
			ofVertex(right + 2, top + 1);
			ofVertex(right - 1, top + 4);
			ofVertex(right - 1, bottom - 4);
			ofVertex(right + 2, bottom - 1);
			ofVertex(left  + 1, bottom - 1);
		ofEndShape();
	}
	else if (aNode->type == "atom" || aNode->type == "numbox"){

		ofSetColor(aNode->borderColor);
		ofBeginShape();
			ofVertex(left,      top);
			ofVertex(right - 4, top);
			ofVertex(right,     top + 4);
			ofVertex(right,     bottom);
			ofVertex(left,      bottom);
		ofEndShape();

		ofSetColor(aNode->backgroundColor);
		// ofSetColor(248, 248, 246);
		ofBeginShape();
			ofVertex(left  + 1, top + 1);
			ofVertex(right - 4, top + 1);
			ofVertex(right - 1, top + 4);
			ofVertex(right - 1, bottom - 1);
			ofVertex(left  + 1, bottom - 1);
		ofEndShape();

		if (aNode->type == "numbox") {

			// ofSetColor(numboxBorderCol);
			ofSetColor(aNode->borderColor);
			ofDrawTriangle(left + 0, top + 0,
			               left + 0, bottom - 0,
			               left + 7, top + aNode->height / 2);

			ofSetColor(backCol);
			ofDrawTriangle(left + 1, top + 2,
			               left + 1, bottom - 2,
			               left + 6, top + aNode->height / 2);

			// ofSetColor(0);
			// this->drawNodeText(aNode->content,
									 // left + 8,
									 // bottom - 3,
									 // // top + Globals::Theme.node.font.height,
									 // // node.bottom - Globals::Theme.node.io.height - 1,
									 // Globals::Theme.node.font.height);
		}
	}
	else {
		
		// if (aNode->type == PdNode::TYPE_CONTROL){
			// ofSetColor(0);
		// }
		// else{
			// ofSetColor(192);
		// }

		// ofSetColor(aNode->control ? 0 : 192);
		// ofSetColor(aNode->valid ? Globals::Theme.node.color.border :
		//                          Globals::Theme.node.color.invalid);
		
		// ofDrawRectangle(*node);

		// ofSetColor(Globals::Theme.node.color.background);
		// ofSetColor(246, 248, 248);
//		if (aNode->className == "tgl"){
//		if (aNode->control){
		// if (aNode->type == PdNode::TYPE_CONTROL){
			// ofSetHexColor(aNode->backgroundColor);
		// }
		// ofDrawRectangle(aNode->x + 1, aNode->y + 1, aNode->width - 2, aNode->height - 2);

		ofSetColor(aNode->borderColor);
		ofDrawRectangle(*aNode);

		ofSetHexColor(aNode->backgroundColor);
		ofDrawRectangle(aNode->x + 1, aNode->y + 1, aNode->width - 2, aNode->height - 2);
	}

	// if (aNode->className == "gatom"){

		// // ofSetPolyMode(OF_POLY_WINDING_NONZERO);
		// // ofSetColor(Globals::Theme.node.color.border);
		// ofSetColor(192);
		// ofBeginShape();
			// ofVertex(left, top);
			// ofVertex(right - 4, top);
			// ofVertex(right, top + 4);
			// ofVertex(right, bottom);
			// ofVertex(left, bottom);
		// ofEndShape();

		// ofSetColor(224);
		// ofBeginShape();
			// ofVertex(left + 1, top + 1);
			// ofVertex(right - 4, top + 1);
			// ofVertex(right - 1, top + 4);
			// ofVertex(right - 1, bottom - 1);
			// ofVertex(left + 1, bottom - 1);
		// ofEndShape();
	// }
	// else if (aNode->className == "nbx"){

		// // ofSetPolyMode(OF_POLY_WINDING_NONZERO);
		// // ofSetColor(Globals::Theme.node.color.border);
		// ofSetColor(0);
		// ofBeginShape();
			// ofVertex(left, top);
			// ofVertex(right - 4, top);
			// ofVertex(right, top + 4);
			// ofVertex(right, bottom);
			// ofVertex(left, bottom);
		// ofEndShape();

		// ofSetColor(252);
		// ofBeginShape();
			// ofVertex(left + 1, top + 1);
			// ofVertex(right - 4, top + 1);
			// ofVertex(right - 1, top + 4);
			// ofVertex(right - 1, bottom - 1);
			// ofVertex(left + 1, bottom - 1);
		// ofEndShape();

		// ofSetColor(0);
		// ofDrawTriangle(left + 0, top + 0,
							// left + 0, bottom - 0,
							// left + 7, top + aNode->height / 2);

		// ofSetColor(252);
		// ofDrawTriangle(left + 1, top + 2,
							// left + 1, bottom - 2,
							// left + 6, top + aNode->height / 2);

		// ofSetColor(0);
		// this->drawNodeText(aNode->content,
								 // left + 8,
								 // bottom - 3,
								 // // top + Globals::Theme.node.font.height,
								 // // node.bottom - Globals::Theme.node.io.height - 1,
								 // Globals::Theme.node.font.height);
	// }
	// else if (aNode->className == "message"){

		// // ofSetColor(Globals::Theme.node.color.border);
		// ofSetColor(192);
		// ofBeginShape();
			// ofVertex(left, top);
			// ofVertex(right + 4, top);
			// ofVertex(right, top + 4);
			// ofVertex(right, bottom - 4);
			// ofVertex(right + 4, bottom);
			// ofVertex(left, bottom);
		// ofEndShape();

		// // ofSetColor(255);
		// ofSetColor(248, 248, 246);
		// ofBeginShape();
			// ofVertex(left + 1, top + 1);
			// ofVertex(right + 2, top + 1);
			// ofVertex(right - 1, top + 4);
			// ofVertex(right - 1, bottom - 4);
			// ofVertex(right + 2, bottom - 1);
			// ofVertex(left + 1, bottom - 1);
		// ofEndShape();
	// }
	// else {
		
		// if (aNode->type == PdNode::TYPE_CONTROL){
			// ofSetColor(0);
		// }
		// else{
			// ofSetColor(192);
		// }

// //		ofSetColor(aNode->control ? 0 : 192);
		// // ofSetColor(aNode->valid ? Globals::Theme.node.color.border :
		// //                          Globals::Theme.node.color.invalid);
		
		// ofDrawRectangle(left, top, aNode->width, aNode->height);

		// // ofSetColor(Globals::Theme.node.color.background);
		// ofSetColor(246, 248, 248);
// //		if (aNode->className == "tgl"){
// //		if (aNode->control){
		// if (aNode->type == PdNode::TYPE_CONTROL){
			// ofSetHexColor(aNode->backgroundColor);
		// }
		// ofDrawRectangle(left + 1, top + 1, aNode->width - 2, aNode->height - 2);
	// }
}


//--------------------------------------------------------------
void Canvas::drawNodeText(PdNode* aNode){

	ofSetColor(0);

	ofScale(0.10f, 0.10f);
	_font.drawString(aNode->text, (aNode->x + aNode->textPosition.x) * 10.0f, (aNode->y + aNode->textPosition.y) * 10.0f);
	ofScale(10.0f, 10.0f);
}


//--------------------------------------------------------------
void Canvas::drawNodeIo(PdIo* aIo){

	ofFill();

	if (aIo->height == 2){
		// == 2 bad way of detecting this, put it in the PdGui and node color

		ofSetColor(0);
		ofDrawRectangle(*aIo);
	}
	else {

		ofSetColor(119);
		ofDrawRectangle(*aIo);

		if (!aIo->signal){
			ofSetColor(255);
			ofDrawRectangle(aIo->x + 1, aIo->y + 1, aIo->width - 2, aIo->height - 2);
		}
	}
}


//--------------------------------------------------------------
void Canvas::onPressed(int aX, int aY, int aId){

	_previousMouse.set(aX, aY);

	ofPoint loc  = this->transformToPdCoordinates(aX, aY);
	PdNode* node = this->getNodeAtPosition(loc.x, loc.y);

	if (!_current->editMode && !node){

		_current->mode = PdCanvas::MODE_DRAG;
	}
	else if (_current->editMode && node && !node->selected && node->outlets.size()){

		_current->mode = PdCanvas::MODE_CONNECT;
		_connectionStart = node->outlets[0];
	}
	else {

		this->sendMouseEvent("mouse", loc);
	}

	// if (!scaling){

		// _pressLoc.set(this->transformLoc(aX, aY, TRANSFORM_MPD_TO_PD));
		// _mouseLoc.set(_pressLoc);

		// Globals::Pd.canvasPressed(_mouseLoc.x, _mouseLoc.y);
	// }
}


//--------------------------------------------------------------
void Canvas::onDragged(int aX, int aY, int aId){

	if (_current->mode == PdCanvas::MODE_DRAG){ // TODO: only if mode_drag

		ofPoint p(aX - _previousMouse.x, aY - _previousMouse.y);

		_current->viewPort.setPosition(_current->viewPort.getPosition() - p / _current->scale);

		_previousMouse.set(aX, aY);

		if (_current->viewPort.x < 0){
			_current->viewPort.x = 0;
		}
		if (_current->viewPort.y < 0){
			_current->viewPort.y = 0;
		}
	}
	else if (_current->mode != PdCanvas::MODE_CONNECT){

		this->sendMouseEvent("motion", this->transformToPdCoordinates(aX, aY));
	}

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

	ofPoint loc = this->transformToPdCoordinates(aX, aY);

	if (_current->mode == PdCanvas::MODE_CONNECT){

		if (auto node = this->getNodeAtPosition(loc.x, loc.y)){

			if ( !node->inlets.size() ){ return; }

			this->sendMouseEvent("mouse", _connectionStart->getCenter());
			this->sendMouseEvent("mouseup", node->inlets[0]->getCenter());
		}
	}
	else {

		this->sendMouseEvent("mouseup", loc);
	}

	_current->mode = PdCanvas::MODE_NONE;
}


//--------------------------------------------------------------
void Canvas::onDoubleClick(int aX, int aY){

	ofPoint p = this->transformToPdCoordinates(aX, aY);

	for (auto node : _current->nodes){
		if (node->inside(p)){
			return;
		}
	}

	string  cmd = _current->id + " editmode " + (_current->editMode ? "0" : "1");

	PdGui::instance().pdsend(cmd);
}


//--------------------------------------------------------------
void Canvas::onPressCancel(){ }


//--------------------------------------------------------------
void Canvas::onAppEvent(AppEvent& aAppEvent){

	switch(aAppEvent.type){

		case AppEvent::TYPE_SCALE_BEGIN:
			break;

		case AppEvent::TYPE_SCALE:
			#ifdef TARGET_ANDROID
			_current->scale *= aAppEvent.value;
			#else
			_current->scale += aAppEvent.value;
			#endif
			_current->viewPort.setSize(this->width / _current->scale, this->height / _current->scale);
			break;

		default:
			break;
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
PdNode* Canvas::getNodeAtPosition(int aX, int aY){

	if (_current){

		for (auto node : _current->nodes){
			if (node->inside(aX, aY)){
				return node;
			}
		}
	}

	return NULL;
}


//--------------------------------------------------------------
void Canvas::sendMouseEvent(string aEventType, ofPoint aLoc){

	string cmd = _current->id + " " + aEventType + " " + ofToString(aLoc.x) + " " + ofToString(aLoc.y) + " 0 0 0";

	PdGui::instance().pdsend(cmd);
}


//--------------------------------------------------------------
ofPoint Canvas::transformToPdCoordinates(float aX, float aY){

	ofPoint result;

	// has to be rounded otherwise pd behaves weirdly when dragging up or left
	result.x = (int)((aX - this->x) / _current->scale + _current->viewPort.x); // Globals::Settings.scale;
	result.y = (int)((aY - this->y) / _current->scale + _current->viewPort.y); // Globals::Settings.scale;

	return result;
}

