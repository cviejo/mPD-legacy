#include "MenuTab.h"
#include "../Globals.h"


//--------------------------------------------------------------
MenuTab::MenuTab() : Tab(DOCK_SIDE_LEFT, 2){

	this->id     = "menu-tab";
	this->width  = Globals::Theme.button.width;
	this->height = ofGetHeight() - Globals::Theme.button.width * 2;
	this->y      = Globals::Theme.button.width;

	_zoomControls.x = this->x;
	_zoomControls.y = this->y + Globals::Theme.button.width;
	
	_zoomControls.add(new Button("bng",       Button::TYPE_DRAG));
	_zoomControls.add(new Button("toggle",    Button::TYPE_DRAG));
	_zoomControls.add(new Button("floatatom", Button::TYPE_DRAG));
	_zoomControls.add(new Button("grid",      Button::TYPE_TOGGLE));// &Globals::Settings.gridActive));

	_editControls.x = this->x;
	_editControls.y = _zoomControls.y + _zoomControls.height + Globals::Theme.button.width;
	
	_editControls.add(new Button("copy"));
	_editControls.add(new Button("paste"));
	_editControls.add(new Button("trash"));

	this->collapsedX = Globals::Theme.tab.collapsedWidth - this->width;

	_animationSpeed  = -40;
}


//--------------------------------------------------------------
void MenuTab::update(){

	if (this->state == STATE_COLLAPSED){
		_zoomControls.setVisibility(false);
		_editControls.setVisibility(false);
	}
	else{
		_zoomControls.setVisibility(true);
		_editControls.setVisibility(true);

		_zoomControls.x = this->x;
		_editControls.x = this->x;

		_zoomControls.update();
		_editControls.update();
	}
}
