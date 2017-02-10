#include "GuiElement.h"
#include "ReverseIterator.h"



JsonTheme GuiElement::Theme;


//--------------------------------------------------------------
GuiElement::GuiElement(string aType){

	this->type = aType;

	ofAddListener(AppEvent::events, this, &GuiElement::onAppEvent);

	if (this->type != ""){

		Json::Value json = GuiElement::Theme[this->type];


		this->setSize(Theme.getScaledValue(this->type, "width"),
		              Theme.getScaledValue(this->type, "height"));

		this->backgroundColor.setHex(Theme.getHex(this->type, "backgroundColor"));
		this->selectionColor .setHex(Theme.getHex(this->type, "selectionColor"));
		this->frontColor     .setHex(Theme.getHex(this->type, "frontColor"));
	}
}


//--------------------------------------------------------------
GuiElement::~GuiElement(){

	ofRemoveListener(AppEvent::events, this, &GuiElement::onAppEvent);

	this->clear();
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
void GuiElement::addChild(GuiElement* aChild){

	this->children.push_back(aChild);

	if (this->width < aChild->getRight()){
		this->width = aChild->getRight();
	}

	if (this->height < aChild->getBottom()){
		this->height = aChild->getBottom();
	}

	this->update();
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
	this->pressedPosition.set(aLoc);

	// for (auto i = this->children.rbegin(); i != this->children.rend(); ++i){
	for (auto& child : reverse(this->children)){

		if (child->touchDown(aLoc - this->getPosition())){
			return true;
		}
	}

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
// void GuiElement::LoadTheme(string aPath){

	// ofBuffer     buffer = ofBufferFromFile(aPath);
	// Json::Reader reader;

	// if (reader.parse(buffer.getText(), GuiElement::Theme)){
		// // TODO: notify / reload gui
	// }
	// else {
		// // TODO: load default / log to console
	// }
// }
