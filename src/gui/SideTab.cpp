#include "SideTab.h"
#include "AppEvent.h"
// #include "../Globals.h"
// #include "../util/AppEvent.h"


//--------------------------------------------------------------
SideTab::SideTab(DockSide aDockingSide) : GuiElement("side-tab"){

	this->side = aDockingSide;

	this->update();
}


//--------------------------------------------------------------
void SideTab::update(){

	auto collapsedWidth = Theme.getScaledValue(this->type, "collapsedWidth");
	auto collapseSpeed  = Theme.getScaledValue(this->type, "collapseSpeed");

	if (this->side == DOCK_SIDE_RIGHT){
		_animationSpeed = collapseSpeed;
		this->collapsedX = ofGetWidth() - collapsedWidth;
	}
	else {
		_animationSpeed = -collapseSpeed;
		this->collapsedX = -this->width + collapsedWidth;
	}
}


//--------------------------------------------------------------
void SideTab::draw(){

	this->updateState();

	GuiElement::draw();
}


//--------------------------------------------------------------
void SideTab::updateState(){

	switch (this->state){

		case STATE_EXPANDED:
			this->x = this->expandedX;
			break;

		case STATE_COLLAPSED:
			this->x = this->collapsedX;
			break;

		case STATE_COLLAPSING:
			_updateNeeded = true;
			this->x += _animationSpeed;
			this->clip();

			if (this->x == this->collapsedX){
				this->state = STATE_COLLAPSED;
				AppEvent event(AppEvent::TYPE_TAB_STATE_CHANGED, this->id, 0);
				ofNotifyEvent(AppEvent::events, event);
			}
			break;

		case STATE_EXPANDING:
			_updateNeeded = true;
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

	for (auto& child : this->children){
		child->visible = this->state != STATE_COLLAPSED;
	}
}


//--------------------------------------------------------------
void SideTab::onAppEvent(AppEvent& aAppEvent){

	_updateNeeded = true;

	if (aAppEvent.type == AppEvent::TYPE_BUTTON_PRESSED && ofIsStringInString(aAppEvent.message, this->id)) {
		this->state = aAppEvent.value ? STATE_EXPANDING : STATE_COLLAPSING;
	}
}


//--------------------------------------------------------------
void SideTab::clip(){

	if (this->side == DOCK_SIDE_RIGHT){
		
		if (this->x >= this->collapsedX){
			this->x = this->collapsedX;
		}
		if (this->x <= this->expandedX){
			this->x = this->expandedX;
		}
	}
	else {

		if (this->x <= this->collapsedX){
			this->x = this->collapsedX;
		}
		if (this->x >= this->expandedX){
			this->x = this->expandedX;
		}
	}
}

