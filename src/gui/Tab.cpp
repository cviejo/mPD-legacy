#include "Tab.h"
#include "../Globals.h"
#include "../util/AppEvent.h"


//--------------------------------------------------------------
void Tab::draw(){

	this->updateState();
	this->update();

	ofSetColor(Globals::Theme.tab.color.background);
	ofDrawRectangle(this->x, this->y, this->width, this->height);
}


//--------------------------------------------------------------
void Tab::updateState(){

	switch (this->state){

		case STATE_EXPANDED:
			this->x = this->expandedX;
			break;

		case STATE_COLLAPSED:
			this->x = this->collapsedX;
			break;

		case STATE_COLLAPSING:
			this->x += _animationSpeed;
			this->clip();

			if (this->x == this->collapsedX){
				this->state	= STATE_COLLAPSED;
				AppEvent event(AppEvent::TYPE_TAB_STATE_CHANGED, this->id, 0);
				ofNotifyEvent(AppEvent::events, event);
			}
			break;

		case STATE_EXPANDING:
			this->x -= _animationSpeed;
			this->clip();

			if (this->x == this->expandedX){
				this->state = STATE_EXPANDED;
				AppEvent event(AppEvent::TYPE_TAB_STATE_CHANGED, this->id, 1);
				ofNotifyEvent(AppEvent::events, event);
			}
			break;
			
		default:
			break;
	}
}


//--------------------------------------------------------------
void Tab::onAppEvent(AppEvent& aAppEvent){

	if (aAppEvent.type == AppEvent::TYPE_BUTTON_PRESSED && ofIsStringInString(aAppEvent.message, this->id)) {
		this->state = aAppEvent.value ? STATE_EXPANDING : STATE_COLLAPSING;
	}
}


//--------------------------------------------------------------
void Tab::clip(){

	if(this->side == DOCK_SIDE_RIGHT){
		
		if(this->x >= this->collapsedX){ 
			this->x = this->collapsedX;
		}
		if(this->x <= this->expandedX) {
			this->x = this->expandedX;
		}
	}
	else{

		if(this->x <= this->collapsedX){
			this->x = this->collapsedX;
		}
		if(this->x >= this->expandedX) {
			this->x = this->expandedX;
		}
	}
}
