#include "ButtonGroup.h"
#include "../Globals.h"


//--------------------------------------------------------------
void ButtonGroup::update(){

	for (auto &btn : this->buttons){

		if(this->orientation == ORIENTATION_VERTICAL){
			btn->x = this->x;
		}
		else{
			btn->y = this->y;
		}
	}
}


//--------------------------------------------------------------
void ButtonGroup::add(Button* aBtn){

	if(this->orientation == ORIENTATION_VERTICAL){

		aBtn->x = this->x;
		aBtn->y = this->y + this->height;

		this->height += aBtn->height + Globals::Theme.button.margin;
	}
	else{
		aBtn->x = this->x + (aBtn->width + Globals::Theme.button.margin) * this->buttons.size();
		aBtn->y = this->y;
	}

	if (aBtn->width  > this->width)  { this->width  = aBtn->width; }
	if (aBtn->height > this->height) { this->height = aBtn->height; }

	this->buttons.push_back(aBtn);
}


//--------------------------------------------------------------
void ButtonGroup::setVisibility(bool aValue){

	this->visible = aValue;
	
	for (auto &btn : this->buttons){
		btn->visible = aValue;
	}
}
