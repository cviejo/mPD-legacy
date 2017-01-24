#include "MainWindow.h"
#include "PdGui.h"
#include "Canvas.h"
#include "SideTab.h"
#include "Button.h"
// #include "Bar.h"


//--------------------------------------------------------------
MainWindow::MainWindow(){

	this->setPosition(0, 0);
	this->setSize(ofGetWidth(), ofGetHeight());

	int         buttonWidth = 120;

	Canvas*     cnv;
	Button*     btn;
	SideTab*    tab;
	GuiElement* bar;


	// canvas
	cnv = new Canvas(this->width, this->height - buttonWidth * 2);
	cnv->setPosition(0, buttonWidth);
	this->children.push_back(cnv);
	// canvas


	// menu-tab
	tab = new SideTab(SideTab::DOCK_SIDE_LEFT);
	tab->id = "menu-tab";
	tab->setPosition(0, buttonWidth);
	tab->setSize(buttonWidth, cnv->height);
	tab->collapsedX = -buttonWidth + 5;
	tab->expandedX = 0;
	this->children.push_back(tab);
	// menu-tab


	// object-tab
	tab = new SideTab(SideTab::DOCK_SIDE_RIGHT);
	tab->id = "object-tab";
	tab->setSize(buttonWidth * 3, cnv->height);
	tab->setPosition(ofGetWidth() - tab->width, buttonWidth);
	tab->collapsedX = ofGetWidth() - 5;
	tab->expandedX = tab->x;
	this->children.push_back(tab);
	// object-tab


	// top-bar
	bar = new GuiElement();
	bar->set(0, 0, ofGetWidth(), buttonWidth);
	bar->id = "top-bar";

	btn = new Button("menu-tab", Button::TYPE_TOGGLE);
	btn->setPosition(0, 0);
	btn->setSize(bar->height, bar->height);
	bar->children.push_back(btn);

	btn = new Button("object-tab", Button::TYPE_TOGGLE);
	btn->setPosition(bar->width - bar->height, 0);
	btn->setSize(bar->height, bar->height);
	bar->children.push_back(btn);

	this->children.push_back(bar);
	// top-bar

	
	// bottom-bar
	bar = new GuiElement();
	bar->set(0, ofGetHeight() - buttonWidth, ofGetWidth(), buttonWidth);
	bar->id = "bottom-bar";

	btn = new Button("edit", Button::TYPE_TOGGLE);
	btn->setPosition(0, 0);
	btn->setSize(bar->height, bar->height);
	bar->children.push_back(btn);

	this->children.push_back(bar);
	// bottom-bar


	cnv->set(PdGui::instance().getCanvases()[0]);
}


//--------------------------------------------------------------
void MainWindow::onAppEvent(AppEvent& aAppEvent){

	switch(aAppEvent.type){

		case AppEvent::TYPE_SCALE_BEGIN:
			break;

		case AppEvent::TYPE_SCALE:
			break;

		case AppEvent::TYPE_BUTTON_PRESSED:
			if (aAppEvent.message == "edit-button"){
			}
			break;

		default:
			break;
	}
}

//--------------------------------------------------------------
// void MainWindow::draw(){

// }

