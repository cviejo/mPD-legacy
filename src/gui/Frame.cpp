#include "Frame.h"
#include "Button.h"
#include "../Globals.h"


//--------------------------------------------------------------
Frame::Frame(void) : GUIElement(5){

	this->width     = ofGetWidth();
	this->height    = Globals::Theme.button.height;
	this->clickable = false;

	auto leftMenuBtn  = new Button("menu-tab",   Button::TYPE_TOGGLE);
	auto rightMenuBtn = new Button("object-tab", Button::TYPE_TOGGLE);
	auto editButton   = new Button("edit",       Button::TYPE_TOGGLE);
	// auto leftMenuBtn  = new Button("left-tab",  Button::TYPE_TOGGLE, &Globals::leftTabOpen);
	// auto rightMenuBtn = new Button("right-tab", Button::TYPE_TOGGLE, &Globals::rightTabOpen);
	// auto editButton   = new Button("edit",      Button::TYPE_TOGGLE, &Globals::editActive);
	// auto zoomInBtn    = new Button("zoom-in");
	// auto zoomOutBtn   = new Button("zoom-out");
	auto settingsBtn  = new Button("settings");

	// rightMenuBtn->x = this->width - leftMenuBtn->width;
	// zoomInBtn->x    = this->width * .5 - Globals::Theme.button.margin * .5 - zoomInBtn->width;
	// zoomOutBtn->x   = this->width * .5 + Globals::Theme.button.margin;
	// settingsBtn->x  = this->width - settingsBtn->width;

	// zoomInBtn->y = zoomOutBtn->y = settingsBtn->y = editButton->y = ofGetHeight() - zoomInBtn->height;

	rightMenuBtn->x = this->width - Globals::Theme.button.height;
	// zoomInBtn->x    = this->width * .5 - Globals::Theme.button.margin * .5 - zoomInBtn->width;
	// zoomOutBtn->x   = this->width * .5 + Globals::Theme.button.margin;
	editButton->x   = (this->width - Globals::Theme.button.height) * .5;
	settingsBtn->x  = this->width - Globals::Theme.button.height;

	rightMenuBtn->y = leftMenuBtn->y = editButton->y = ofGetHeight() - Globals::Theme.button.height;
}


//--------------------------------------------------------------
void Frame::draw(){

	ofSetColor(Globals::Theme.frame.color.background);

	ofDrawRectangle(0, 0, this->width, this->height);
	ofDrawRectangle(0, ofGetHeight() - this->height, this->width, this->height);
}
