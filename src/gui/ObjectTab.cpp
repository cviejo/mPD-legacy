#include "ObjectTab.h"
#include "../Globals.h"


//--------------------------------------------------------------
ObjectTab::ObjectTab() : Tab(DOCK_SIDE_RIGHT, 2){

	this->id     = "object-tab";
	this->height = ofGetHeight() - Globals::Theme.button.width * 2;
	this->width  = _indexScroller.width + _objectScroller.width;
	this->y      = Globals::Theme.button.width;

	_indexScroller.x  = this->x;
	_indexScroller.y  = this->y;
	_objectScroller.x = this->x;
	_objectScroller.y = this->y;

	_margin = this->expandedX + this->width - this->collapsedX;

	this->collapsedX = ofGetWidth() - Globals::Theme.tab.collapsedWidth;
	this->expandedX  = ofGetWidth() - this->width;

	_animationSpeed = 40;
}


//--------------------------------------------------------------
void ObjectTab::setContent(vector<string> aItems){

	_objectScroller.setContent(aItems);
}


//--------------------------------------------------------------
void ObjectTab::update(){

	if(this->state == STATE_COLLAPSED){
		_indexScroller .visible = false;
		_objectScroller.visible = false;
	}
	else{
		_indexScroller .visible = true;
		_objectScroller.visible = true;
		
		_indexScroller .x = this->x + this->width - _indexScroller.width;
		_objectScroller.x = this->x + Globals::DPI * .007f;;
	}
}
