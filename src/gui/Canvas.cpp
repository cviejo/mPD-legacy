#include "Canvas.h"
#include "../Globals.h"
#include "canvas-renderers/CanvasRenderer.h"


//--------------------------------------------------------------
Canvas::Canvas() : GUIElement(0){

	_renderer = new CanvasRenderer();
	// _renderer.font = _font;

	//todo: tidy up here
	_font.load(50);
	// _font.load(Globals::Theme.node.font.height);
	_consoleFont.load(Globals::Theme.console.font.height);

	_renderer->font = _font;

	float lineHeight = _font.lineHeight / 50.0f * Globals::Theme.node.font.height;

	ofLog(OF_LOG_VERBOSE, "[lin] " +  ofToString(lineHeight) + ".");

	Globals::Pd.setFontSize(7, lineHeight);
	// Globals::Pd.setFontSize(6, 8);
	Globals::Pd.setIOHeight(Globals::Theme.node.io.height);

	this->x      = Globals::Theme.tab.collapsedWidth;
	this->y      = Globals::Theme.button.height;
	this->width  = ofGetWidth();
	this->height = ofGetHeight() - Globals::Theme.button.height * 2;

	this->initGrid();
}

//---------------------------VIRTUAL--------------------------//
//--------------------------------------------------------------
void Canvas::draw(){

	PdNode canvas = Globals::Pd.getCanvas();

	ofPushMatrix();
	ofTranslate(this->x, this->y);
	ofScale(Globals::Settings.scale, Globals::Settings.scale);

	this->drawGrid();
	this->drawConsole();

	ofTranslate(_draggedLoc + _offsetLoc);

	this->drawConnections(canvas);
	this->drawCanvas(canvas);
	this->drawConnecting();
	this->drawRegion();

	ofPopMatrix();
}


//--------------------------------------------------------------
void Canvas::onAppEvent(AppEvent& aAppEvent){

	if (aAppEvent.type == AppEvent::TYPE_BUTTON_PRESSED){

		if (aAppEvent.message == "edit-button"){

			Globals::Pd.setCanvasEditMode(aAppEvent.value);
		}
		else if (aAppEvent.message == "grid-button"){

			Globals::Settings.gridActive = aAppEvent.value;
			Globals::Pd.setCanvasGridMode(aAppEvent.value);
		}
		else if (aAppEvent.message == "settings-button"){
		}
	}
	else if(aAppEvent.type == AppEvent::TYPE_CREATE_OBJECT){

		PdNode node;

		ofPoint loc = this->transformLoc(aAppEvent.x, aAppEvent.y, TRANSFORM_MPD_TO_PD);

		node.className = aAppEvent.message;
		node.x         = loc.x;
		node.y         = loc.y;

		Globals::Pd.canvasCreateObject(node);
	}
}


//--------------------------------------------------------------
void Canvas::onPressed(int aX, int aY, int aId){

	if (!scaling){

		_pressLoc.set(this->transformLoc(aX, aY, TRANSFORM_MPD_TO_PD));
		_mouseLoc.set(_pressLoc);

		Globals::Pd.canvasPressed(_mouseLoc.x, _mouseLoc.y);
	}
}


//--------------------------------------------------------------
void Canvas::onDragged(int aX, int aY, int aId){

	_mouseLoc.set(this->transformLoc(aX, aY, TRANSFORM_MPD_TO_PD));

	CanvasMode mode = Globals::Pd.getCanvasMode();

	if (mode != CANVAS_MODE_CONNECT){

		if (scaling || (!Globals::Pd.getCanvasEditMode() && !Globals::Pd.getNodePressed() && mode == CANVAS_MODE_NONE)){
		// if (!Globals::Pd.getCanvasEditMode() && mode == CANVAS_MODE_NONE){

			_draggedLoc.x = _mouseLoc.x - _pressLoc.x;
			_draggedLoc.y = _mouseLoc.y - _pressLoc.y;

			this->clipOffset();
		}
		else {

			Globals::Pd.canvasDragged(_mouseLoc.x, _mouseLoc.y);
		}
	}
}


//--------------------------------------------------------------
void Canvas::onReleased(int aX, int aY, int aId){

	_mouseLoc.set(this->transformLoc(aX, aY, TRANSFORM_MPD_TO_PD));

	_offsetLoc.x += _draggedLoc.x;
	_offsetLoc.y += _draggedLoc.y;

	_pressLoc  .set(-1, -1);
	_draggedLoc.set(0, 0);

	if (!scaling){
		Globals::Pd.canvasReleased(_mouseLoc.x, _mouseLoc.y);
	}
}


//--------------------------------------------------------------
void Canvas::onDoubleClick(int aX, int aY){

	if (!Globals::Pd.getNodePressed()){
		auto editMode = Globals::Pd.getCanvasEditMode();
		Globals::Pd.setCanvasEditMode(!editMode);
	}
}


//--------------------------------------------------------------
void Canvas::onPressCancel(){
}


//--------------------------------------------------------------
void Canvas::print(const string& aMessage) {

	if (_logs.size() == (int)(this->height / _consoleFont.lineHeight)){
		_logs.erase(_logs.begin());
	}

	_logs.push_back(aMessage);
}


//--------------------------------------------------------------
void Canvas::setFocus(int aX, int aY){

	auto focusLoc  = this->transformLoc(aX, aY, TRANSFORM_MPD_TO_PD);
	auto offsetLoc = this->transformLoc(this->width * .5f, this->height * .5f, TRANSFORM_MPD_TO_PD);

	_offsetLoc.set(focusLoc - offsetLoc);

	this->clipOffset();
}


//--------------------------------------------------------------
void Canvas::clipOffset(){

	if(_draggedLoc.x + _offsetLoc.x > 0){
		_offsetLoc.x  = 0;
		_draggedLoc.x = 0;
	}

	if(_draggedLoc.y + _offsetLoc.y > 0){
		_offsetLoc.y  = 0;
		_draggedLoc.y = 0;
	}
}


//--------------------------------------------------------------
void Canvas::initGrid(){

	int stepWidth  = Globals::Theme.grid.cell.width  + 0.5f;
	int stepHeight = Globals::Theme.grid.cell.height + 0.5f;
	int gridWidth  = this->width  + stepWidth * 3;
	int gridHeight = this->height + stepWidth * 3;

	_grid.allocate(gridWidth, gridHeight, GL_RGBA);
	_grid.begin();

	ofClear(255,255,255, 0);
	ofBackground(Globals::Theme.canvas.color.background);
	ofSetColor(Globals::Theme.grid.color.front);

	for(int i = 0; i < gridWidth; i += stepWidth){
		for(int j = 0; j < gridHeight; j += stepHeight){
			ofDrawCircle(i, j, 1);
		}
	}

	_grid.end();

	Globals::Pd.setCanvasGridMode(true);
	Globals::Pd.setCanvasGridSize(Globals::Theme.grid.cell.width, 
	                              Globals::Theme.grid.cell.height);
}


//--------------------------------------------------------------
void Canvas::drawGrid(){

	if(!Globals::Settings.gridActive || Globals::Settings.scale < 1){ return; };

	ofSetColor(Globals::Theme.grid.color.front);

	_grid.draw((int)((_offsetLoc.x + _draggedLoc.x)) % Globals::Theme.grid.cell.width,
	           (int)((_offsetLoc.y + _draggedLoc.y)) % Globals::Theme.grid.cell.height);
}


//--------------------------------------------------------------
void Canvas::drawConsole(){

	ofSetColor(Globals::Theme.console.color.front);

	for(int i = _logs.size()-1; i >= 0; i--){
		_consoleFont.draw(_logs[i], 0, _consoleFont.lineHeight * (i + 1));
	}
}


//--------------------------------------------------------------
void Canvas::drawRegion(){

	if(Globals::Pd.getCanvasEditMode() && Globals::Pd.getCanvasMode() == CANVAS_MODE_REGION){

		ofSetColor(100);
		ofNoFill();
		ofDrawRectangle(_pressLoc, _mouseLoc.x - _pressLoc.x, _mouseLoc.y - _pressLoc.y);

		ofSetColor(100, 100);
		ofFill();
		ofDrawRectangle(_pressLoc, _mouseLoc.x - _pressLoc.x, _mouseLoc.y - _pressLoc.y);
	}
}


//--------------------------------------------------------------
void Canvas::drawCanvas(PdNode& aCanvas){

	_renderer->drawCanvas(aCanvas);

// 	ofFill();

// 	for (auto node : aCanvas.nodes){

// 		if (node.comment){
// 			this->drawNodeText(node);
// 			continue;
// 		}

		// if (node.selected){
		// 	ofSetColor(240, 0, 0);
		// 	ofDrawRectangle(node.x - 3, node.y - 3, node.width + 6, node.height + 6);
		// }

// 		// if (node.className == "message" || node.className == "gatom" || node.className == "nbx"){
// //		if (node.className == "message" || node.className == "nbx"){
// 		if (node.className == "canvas" && node.graph){

// 			if (node.graph){
// 				this->drawControlBackground(node);
// 				this->drawCanvas(node);
// 			}
// 		}
// 		else if (node.className == "message"){

// 			ofSetColor(0);

// 			ofBeginShape();
// 				ofVertex(node.x, node.y);
// 				ofVertex(node.right + 4, node.y);
// 				ofVertex(node.right, node.y + 4);
// 				ofVertex(node.right, node.bottom - 4);
// 				ofVertex(node.right + 4, node.bottom);
// 				ofVertex(node.x, node.bottom);
// 			ofEndShape();

// 			ofSetColor(255);

// 			ofBeginShape();
// 				ofVertex(node.x + 1, node.y + 1);
// 				ofVertex(node.right + 2, node.y + 1);
// 				ofVertex(node.right - 1, node.y + 4);
// 				ofVertex(node.right - 1, node.bottom - 4);
// 				ofVertex(node.right + 2, node.bottom - 1);
// 				ofVertex(node.x + 1, node.bottom - 1);
// 			ofEndShape();
// 		}
// 		else if (node.className == "nbx"){

// 			this->drawControlBackground(node);
// 			ofSetColor(Globals::Theme.node.color.front);
// 			this->drawNodeText(node);

// 			if (node.className == "gatom" || node.className == "nbx"){
// 				ofDrawTriangle(node.right - 11, node.y, node.right, node.y, node.right, node.y + 7);
// 			}
// 		}
// 		else if (node.className == "gatom"){

// 			// ofSetPolyMode(OF_POLY_WINDING_NONZERO);
			
// 			ofSetColor(0);

// 			ofBeginShape();
// 				ofVertex(node.x, node.y);
// 				ofVertex(node.right - 4, node.y);
// 				ofVertex(node.right, node.y + 4);
// 				ofVertex(node.right, node.bottom);
// 				ofVertex(node.x, node.bottom);
// 			ofEndShape();
			
// 			ofSetColor(255);
			
// 			ofBeginShape();
// 				ofVertex(node.x + 1, node.y + 1);
// 				ofVertex(node.right - 4, node.y + 1);
// 				ofVertex(node.right - 1, node.y + 4);
// 				ofVertex(node.right - 1, node.bottom - 1);
// 				ofVertex(node.x + 1, node.bottom - 1);
// 			ofEndShape();
// 		}
// 		else if (node.className == "cnv"){

// 			this->drawControlBackground(node);
// 			this->drawNodeLabel(node);
// 			ofNoFill();
// 			ofSetColor(Globals::Theme.control.color.background);
// 			ofSetHexColor(node.backgroundColor);
// 			ofDrawRectangle(node.x, node.y, node.backgroundWidth, node.backgroundHeight);
// 		}
// 		else if (node.className == "canvas" && node.graph){

// 			if (node.graph){
// 				this->drawControlBackground(node);
// 				this->drawCanvas(node);
// 			}
// 		}
// 		else if (node.control){

// 			this->drawControl(node);
// 		}
// 		else {

// 			this->drawNodeBackground(node);
// 			this->drawNodeText(node);
// 		}

// 		this->drawIO(node, true);
// 		this->drawIO(node, false);
// 	}


// 	if (Globals::Pd.getCanvasMode() == CANVAS_MODE_CONNECT){
// 		for (auto node : aCanvas.nodes){
// 			if (node.selected){
// 				ofPoint sourceLoc = Globals::Pd.getConnectSourceLoc();
// 				ofDrawLine(sourceLoc, _mouseLoc);
// 				break;
// 			}
// 		}
// 	}
}


//--------------------------------------------------------------
void Canvas::drawConnections(PdNode& aCanvas){

	for (auto elem : aCanvas.connections){

		if (elem.signal){
			ofSetColor(Globals::Theme.connection.color.signal);
			ofSetLineWidth(2 * Globals::Settings.scale);
		}
		else {
			ofSetColor(Globals::Theme.connection.color.control);
			ofSetLineWidth(Globals::Settings.scale);
		}
		ofDrawLine(elem.x1, elem.y1 - 1, elem.x2, elem.y2 + 1);
	}
}


//--------------------------------------------------------------
void Canvas::drawConnecting(){

	if (Globals::Pd.getCanvasMode() == CANVAS_MODE_CONNECT){
		// ofDrawLine(_pressLoc, _mouseLoc);
	}
}


//--------------------------------------------------------------
void Canvas::drawControl(PdNode& aNode){

	this->drawControlBackground(aNode);

	ofFill();

	int barCorner = 2;

	if (aNode.className == "bng"){

		// ofSetColor(aNode.value? Globals::Theme.control.color.front :
		//                         Globals::Theme.control.color.disabled);

		ofSetHexColor(aNode.frontColor);
		ofDrawEllipse(aNode.x + aNode.width  * .5f,
		              aNode.y + aNode.height * .5f,
		              aNode.width  - barCorner * 2,
		              aNode.height - barCorner * 2);
	}
	else if (aNode.className == "tgl"){

		// ofSetColor(aNode.value? Globals::Theme.control.color.front :
		//                         Globals::Theme.control.color.disabled);
		ofSetHexColor(aNode.frontColor);
		ofDrawLine(aNode.x + barCorner, aNode.y + barCorner, aNode.right - barCorner, aNode.bottom - barCorner);
		ofDrawLine(aNode.x + barCorner, aNode.bottom - barCorner, aNode.right - barCorner, aNode.y + barCorner);
	}
	else if (aNode.className == "vsl"){

		int valueY = aNode.bottom - aNode.value;
		// ofSetColor(Globals::Theme.control.color.front);
		ofSetHexColor(aNode.frontColor);
		ofDrawRectangle(aNode.x, valueY, aNode.width, 1);
	}
	else if (aNode.className == "hsl"){

		int valueX = aNode.x + aNode.value;
		// ofSetColor(Globals::Theme.control.color.front);
		ofSetHexColor(aNode.frontColor);
		ofDrawRectangle(valueX, aNode.y, 1, aNode.height);
	}
	else if (aNode.className == "hradio"){

		int separation = aNode.width / aNode.valueMax;

		ofSetHexColor(aNode.frontColor);
		ofDrawRectangle(aNode.x + separation * aNode.value, aNode.y + 1, aNode.height, aNode.height - 2);

		ofSetColor(Globals::Theme.control.color.border);
		ofDrawRectangle(aNode.x, aNode.y, 1, aNode.height);
		for(int i = 1; i < aNode.valueMax; i++){
			ofDrawRectangle(aNode.x + i * separation, aNode.y, 1, aNode.height);
		}
		ofDrawRectangle(aNode.x + aNode.valueMax * separation - 1, aNode.y, 1, aNode.height);
	}
	else if (aNode.className == "vradio"){

		int separation = aNode.height / aNode.valueMax;

		ofSetHexColor(aNode.frontColor);
		ofDrawRectangle(aNode.x + 1, aNode.y + separation * aNode.value, aNode.width - 2, aNode.width);

		ofSetColor(Globals::Theme.control.color.border);
		// ofSetHexColor(aNode.frontColor);
		ofDrawRectangle(aNode.x, aNode.y, aNode.width, 1);
		for(int i = 1; i < aNode.valueMax; i++){
			ofDrawRectangle(aNode.x, aNode.y + i * separation - 1, aNode.width, 1);
		}
		ofDrawRectangle(aNode.x, aNode.y + aNode.valueMax * separation - 1, aNode.width, 1);
	}

	this->drawNodeLabel(aNode);
}


//--------------------------------------------------------------
void Canvas::drawIO(PdNode& aNode, bool aInlet){

	int          ioWidth  = 7;
	int          ioHeight = aNode.control ? 1 : 3;//Globals::Theme.node.io.height;
	int          ioY      = aInlet ? aNode.y : aNode.bottom - ioHeight;
	vector<bool> io       = aInlet ? aNode.inlets : aNode.outlets;
	int          count    = io.size();
	
	if (count){

		int interval = count > 1 ? (aNode.width - ioWidth) / (count-1) : 0;

		for(int i = 0; i < count; i++){

			int ioX = aNode.x + interval * i;

			if(count > 1 && i == count - 1){
				ioX = aNode.right - ioWidth;
			}
			
			ofSetColor(io[i] ? Globals::Theme.connection.color.signal :
			                   Globals::Theme.connection.color.control);

			ofDrawRectangle(ioX, ioY, ioWidth, ioHeight);
		}
	}
}


//--------------------------------------------------------------
void Canvas::drawNodeText(PdNode& aNode){

	ofSetColor(Globals::Theme.node.color.front);
	ofScale(0.10f, 0.10f);
	_font.draw(aNode.content, (aNode.x + 1) * 10.0f, (aNode.y + 4 + Globals::Theme.node.font.height) * 10.0f);
	ofScale(10.0f, 10.0f);
}


//--------------------------------------------------------------
void Canvas::drawNodeLabel(PdNode& aNode){

	// float b = (float)_font.size / (float)aNode.labelFontSize;
	float b = (float)_font.size / (float)aNode.labelFontSize;

	ofSetColor(Globals::Theme.node.color.front);
	ofScale(0.10f, 0.10f);
	if (aNode.label != "empty"){
		_font.draw(aNode.label, (aNode.x + aNode.labelLoc.x) * 10.0f, (aNode.y + aNode.labelLoc.y) * 10.0f);
	}
	ofScale(10.0f, 10.0f);
}


//--------------------------------------------------------------
void Canvas::drawNodeBackground(PdNode& aNode){

	ofFill();

	ofSetColor(aNode.valid ? Globals::Theme.node.color.border : 
	                         Globals::Theme.node.color.invalid);
	ofDrawRectangle(aNode.x, aNode.y, aNode.width, aNode.height);

	ofSetColor(Globals::Theme.node.color.background);
	ofDrawRectangle(aNode.x + 1,
	                aNode.y + Globals::Theme.node.io.height,
	                aNode.width - 2,
	                aNode.height - Globals::Theme.node.io.height * 2);
}


//--------------------------------------------------------------
void Canvas::drawControlBackground(PdNode& aNode){

	ofFill();

	ofSetColor(Globals::Theme.control.color.border);

	// ofSetHexColor(node.backgroundColor);
	ofDrawRectangle(aNode.x, aNode.y, aNode.width, aNode.height);

	ofSetHexColor(aNode.backgroundColor);
	ofDrawRectangle(aNode.x + 1, aNode.y + 1, aNode.width - 2, aNode.height - 2);
}


//--------------------------------------------------------------
ofPoint Canvas::transformLoc(float aX, float aY, CoordinateTransformation aTransform){

	ofPoint result;

	if (aTransform == TRANSFORM_PD_TO_MPD){
		result.set(aX * Globals::Settings.scale, aY * Globals::Settings.scale);
	}
	else {
		result.x = (aX - this->x) / Globals::Settings.scale - _offsetLoc.x; // Globals::Settings.scale;
		result.y = (aY - this->y) / Globals::Settings.scale - _offsetLoc.y; // Globals::Settings.scale;
	}

	return result;
}
