#include "Canvas.h"


//--------------------------------------------------------------
Canvas::Canvas(){

	_viewPort.setPosition(0, 0);
	_scale = 1.f;

	this->x      = 0;
	this->y      = 0;
	// this->width  = ofGetWidth();
	// this->height = ofGetHeight();
	this->width  = 400;
	this->height = 400;

	_viewPort.setSize(this->width * _scale, this->height * _scale);
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
}


//--------------------------------------------------------------
void Canvas::initGrid(){

	int stepWidth  = 20;//Globals::Theme.grid.cell.width  + 0.5f;
	int stepHeight = 20;//Globals::Theme.grid.cell.height + 0.5f;
	int gridWidth  = this->width  + stepWidth * 3;
	int gridHeight = this->height + stepWidth * 3;

	_grid.allocate(gridWidth, gridHeight, GL_RGBA);
	_grid.begin();

	ofClear(255,255,255, 0);
	// ofBackground(Globals::Theme.canvas.color.background);
	// ofSetColor(Globals::Theme.canvas.color.background);
	ofSetColor(0);
	// ofBackground(0,0,255);
	// ofSetColor(0,0,255);
	// ofDrawRectangle(0, 0, gridWidth, gridHeight);
	// ofSetColor(Globals::Theme.grid.color.front);
	ofSetColor(200);

	for(int i = 0; i < gridWidth; i += stepWidth){
		for(int j = 0; j < gridHeight; j += stepHeight){
			ofDrawCircle(i, j, 1);
		}
	}

	_grid.end();

	// Globals::Pd.setCanvasGridMode(true);
	// Globals::Pd.setCanvasGridSize(Globals::Theme.grid.cell.width, 
											// Globals::Theme.grid.cell.height);
}


//---------------------------VIRTUAL--------------------------//
//--------------------------------------------------------------
void Canvas::draw(){

	ofSetColor(0);
	ofDrawRectangle(*this);

	ofPushMatrix();
	ofTranslate(this->x, this->y);
	ofScale(_scale, _scale);

	// _grid.draw((int)(_offsetLoc.x + _draggedLoc.x) % Globals::Theme.grid.cell.width,
	//            (int)(_offsetLoc.y + _draggedLoc.y) % Globals::Theme.grid.cell.height);

	ofSetColor(200);
	_grid.draw(0, 0);

	vector<PdCanvas*> canvases = PdGui::instance().getCanvases();

	ofSetColor(255);

	for (auto& canvas : canvases){

		for (auto node : canvas->nodes){

			if (_viewPort.intersects(*node)){

				this->drawNodeBackground(node);

				ofSetColor(0);
				ofDrawBitmapString(node->text, node->getBottomLeft());

				for (auto inlet : node->inlets){
					this->drawNodeIo(inlet);
				}

				for (auto outlet : node->outlets){
					this->drawNodeIo(outlet);
				}
			}
		}

		for (auto conn : canvas->connections){
			ofSetColor(119);
			ofSetLineWidth(2);
			ofDrawLine(conn->x1, conn->y1, conn->x2, conn->y2);
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
void Canvas::drawNodeBackground(PdNode* aNode){

	ofFill();

	int top    = aNode->getTop();
	int left   = aNode->getLeft();
	int right  = aNode->getRight();
	int bottom = aNode->getBottom();

	int borderCol = 204;
	int numboxBorderCol = 168;
	int backCol   = 240;


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
			ofVertex(left + 1,  top + 1);
			ofVertex(right + 2, top + 1);
			ofVertex(right - 1, top + 4);
			ofVertex(right - 1, bottom - 4);
			ofVertex(right + 2, bottom - 1);
			ofVertex(left + 1,  bottom - 1);
		ofEndShape();
	}
	else if (aNode->type == "atom" || aNode->type == "numbox"){

		ofSetColor(borderCol);
		ofBeginShape();
			ofVertex(left,      top);
			ofVertex(right - 4, top);
			ofVertex(right,     top + 4);
			ofVertex(right,     bottom);
			ofVertex(left,      bottom);
		ofEndShape();

		ofSetColor(backCol);
		// ofSetColor(248, 248, 246);
		ofBeginShape();
			ofVertex(left + 1,  top + 1);
			ofVertex(right - 4, top + 1);
			ofVertex(right - 1, top + 4);
			ofVertex(right - 1, bottom - 1);
			ofVertex(left + 1,  bottom - 1);
		ofEndShape();

		if (aNode->type == "numbox") {

			ofSetColor(numboxBorderCol);
			ofDrawTriangle(left + 0, top + 0,
			               left + 0, bottom - 0,
			               left + 7, top + aNode->height / 2);

			ofSetColor(backCol);
			ofDrawTriangle(left + 1, top + 2,
			               left + 1, bottom - 2,
			               left + 6, top + aNode->height / 2);

			ofSetColor(0);
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

		ofSetColor(borderCol);
		ofDrawRectangle(*aNode);

		if (aNode->selected){
			ofSetColor(255,0,0);
		}
		else {
			ofSetHexColor(aNode->backgroundColor);
		}

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
void Canvas::drawNodeIo(PdIo& aIo){

	ofFill();

	if (aIo.height == 1){

		ofSetColor(0);
		ofDrawRectangle(aIo);
	}
	else {

		ofSetColor(119);
		ofDrawRectangle(aIo);

		if (!aIo.signal){
			ofSetColor(255);
			ofDrawRectangle(aIo.x + 1, aIo.y + 1, aIo.width - 2, aIo.height - 2);
		}
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

	switch(aAppEvent.type){

		case AppEvent::TYPE_SCALE_BEGIN:
			break;


		case AppEvent::TYPE_SCALE:
			#ifdef TARGET_ANDROID
			_scale *= aAppEvent.value;
			#else
			_scale += aAppEvent.value;
			#endif
			_viewPort.setSize(this->width / _scale, this->height / _scale);
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

// void Canvas::setFocus(int aX, int aY){

	// auto focusLoc  = this->transformToPdCoordinates(aX, aY);
	// auto offsetLoc = this->transformToPdCoordinates(this->width * .5f, this->height * .5f);

	// _viewPort.setPosition(focusLoc - offsetLoc);

	// // this->clipOffset();
// }


//--------------------------------------------------------------

// void Canvas::clipOffset(){

	// if(_draggedLoc.x + _offsetLoc.x > 0){
		// _offsetLoc .x = 0;
		// _draggedLoc.x = 0;
	// }

	// if(_draggedLoc.y + _offsetLoc.y > 0){
		// _offsetLoc .y = 0;
		// _draggedLoc.y = 0;
	// }
// }


//--------------------------------------------------------------
ofPoint Canvas::transformToPdCoordinates(float aX, float aY){

	ofPoint result;

	result.x = (aX - this->x) / _scale - _viewPort.x; // Globals::Settings.scale;
	result.y = (aY - this->y) / _scale - _viewPort.y; // Globals::Settings.scale;

	return result;
}

