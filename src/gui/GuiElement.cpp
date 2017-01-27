#include "GuiElement.h"
#include "ReverseIterator.h"


//--------------------------------------------------------------
GuiElement::GuiElement(){

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

	// TODO: backgroundColor;
	ofSetColor(34, 35, 38);

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

