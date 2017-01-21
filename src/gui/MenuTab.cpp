
#include "MenuTab.h"
// #include "..lobals.h"


//--------------------------------------------------------------
MenuTab::MenuTab() : SideTab(DOCK_SIDE_LEFT){

	this->id     = "menu-tab";
	// this->width  = Globals::Theme.button.width;
	this->width  = 30;
	// this->height = ofGetHeight() - Globals::Theme.button.width * 2;
	this->height = ofGetHeight();
	// this->y      = Globals::Theme.button.width;
	this->y      = 0;
	// this->x      = 0;

	// _zoomControls.x = this->x;
	// _zoomControls.y = this->y + Globals::Theme.button.width;
	
	// _zoomControls.add(new Button("bng",       Button::TYPE_DRAG));
	// _zoomControls.add(new Button("toggle",    Button::TYPE_DRAG));
	// _zoomControls.add(new Button("floatatom", Button::TYPE_DRAG));
	// _zoomControls.add(new Button("undo"));
	// _zoomControls.add(new Button("grid",      Button::TYPE_TOGGLE));// &Globals::Settings.gridActive));

	// _editControls.x = this->x;
	// _editControls.y = _zoomControls.y + _zoomControls.height + Globals::Theme.button.width;
	
	// _editControls.add(new Button("copy"));
	// _editControls.add(new Button("paste"));

	this->collapsedX = -25;

	_animationSpeed  = -10;
}


//--------------------------------------------------------------
void MenuTab::update(){

	if (this->state == STATE_COLLAPSED){
		// _zoomControls.setVisibility(false);
		// _editControls.setVisibility(false);
	}
	else{
		// _zoomControls.setVisibility(true);
		// _editControls.setVisibility(true);

		// _zoomControls.x = this->x;
		// _editControls.x = this->x;

		// _zoomControls.update();
		// _editControls.update();
	}
}
