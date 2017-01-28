#include "GuiElement.h"
#include "ReverseIterator.h"



Json::Value GuiElement::theme;


//--------------------------------------------------------------
GuiElement::GuiElement(string aType){

	this->type = aType;

	// TODO: real dpi
	float dpi = 150.0f;
	// float dpi = 444.0f;

	if (this->type != ""){

		Json::Value json = GuiElement::theme[this->type];

		auto temp = ofToInt(json["backgroundColor"].asString());

		this->backgroundColor.setHex(ofHexToInt(json["backgroundColor"].asString()));
		this->frontColor.setHex(ofHexToInt(json["frontColor"].asString()));
		this->selectionColor.setHex(ofHexToInt(json["selectionColor"].asString()));
	}

	ofAddListener(AppEvent::events, this, &GuiElement::onAppEvent);
}


//--------------------------------------------------------------
void GuiElement::draw(){

	this->drawBackground();

	this->drawChildren();
}


//--------------------------------------------------------------
void GuiElement::drawChildren(){

	ofPushMatrix();

	ofTranslate(this->x, this->y);

	for (auto& child : this->children){

		if (child->visible){
			child->draw();
		}
	}

	ofPopMatrix();
}


//--------------------------------------------------------------
void GuiElement::drawBackground(){

	ofSetColor(this->backgroundColor);

	ofDrawRectangle(*this);
}


//--------------------------------------------------------------
void GuiElement::clear(){

	for (auto& item : this->children){
		delete item;
	}

	this->children.clear();
}


//--------------------------------------------------------------
bool GuiElement::touchDown(ofPoint aLoc){

	if (!this->touchTest(aLoc)){ return false; }

	this->pressed = true;
	this->pressedPoint.set(aLoc);

	// for (auto i = this->children.rbegin(); i != this->children.rend(); ++i){
	for (auto& child : reverse(this->children)){

		if (child->touchDown(aLoc - this->getPosition())){
			return true;
		}
	}

	ofLogVerbose() << aLoc.x << " " << aLoc.y;

	this->onPressed(aLoc.x, aLoc.y, 0);

	return true;
}


//--------------------------------------------------------------
void GuiElement::touchUp(ofPoint aLoc){

	if (!this->pressed){ return; }

	for (auto& child : reverse(this->children)){

		if (child->pressed){
			child->touchUp(aLoc - this->getPosition());
			return;
		}
	}

	if (this->inside(aLoc)){
		this->onClick();
	}

	this->onReleased(aLoc.x, aLoc.y, 0);
	// this->onReleased(aLoc);
	this->pressed = false;
}


//--------------------------------------------------------------
void GuiElement::touchMoved(ofPoint aLoc){

	if (!this->pressed){ return; }

	for (auto& child : reverse(this->children)){

		if (child->pressed){
			child->touchMoved(aLoc - this->getPosition());
			return;
		}
	}

	this->onDragged(aLoc.x, aLoc.y, 0);
}


//--------------------------------------------------------------
bool GuiElement::touchTest(ofPoint aLoc){

	return this->visible && this->clickable && this->inside(aLoc);
}


//--------------------------------------------------------------
void GuiElement::LoadTheme(string aPath){

	ofBuffer     buffer = ofBufferFromFile(aPath);
	Json::Reader reader;

	if (reader.parse(buffer.getText(), GuiElement::theme)){
		Json::Value val = GuiElement::theme["bar"];
		ofLogVerbose() << val["backgroundColor"].asString();
		ofLogVerbose() << "okkkk";
		// TODO: notify / reload gui
	}
	else {
		ofLogVerbose() << "dafuq";
		// TODO: load default / log to console
	}
}

