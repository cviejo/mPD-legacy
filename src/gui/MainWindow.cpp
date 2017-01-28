#include "MainWindow.h"
#include "PdGui.h"
#include "Canvas.h"
#include "SideTab.h"
#include "Button.h"
#include "IndexScroller.h"
// #include "Bar.h"



//--------------------------------------------------------------
MainWindow::MainWindow(){

	ofAddListener(PdEvent::events, this, &MainWindow::onPdEvent);

	this->setPosition(0, 0);
	this->setSize(ofGetWidth(), ofGetHeight());

	// TODO: real dpi
	float dpi = 150.0f;
	// float dpi = 444.0f;

	int buttonWidth    = (int)(dpi / 100.0f * 30.0f);
	int collapsedWidth = (int)(dpi / 100.0f * 5.0f);

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
	tab->collapsedX = -buttonWidth + collapsedWidth;
	tab->expandedX = 0;

	vector<string> buttons = { "trash", "undo", "paste", "copy" };

	for (int i = 0; i < buttons.size(); i++) {
		btn = new Button(buttons[i]);
		btn->setPosition(0, tab->height - buttonWidth * (i + 2));
		btn->setSize(tab->width, tab->width);
		tab->children.push_back(btn);
	}

	btn = new Button("grid", Button::TYPE_TOGGLE);
	btn->setPosition(0, tab->height - buttonWidth * 7);
	btn->setSize(tab->width, tab->width);
	tab->children.push_back(btn);

	this->children.push_back(tab);
	// menu-tab


	// object-tab
	tab = new SideTab(SideTab::DOCK_SIDE_RIGHT);
	tab->id = "object-tab";
	tab->setSize(buttonWidth * 3, cnv->height);
	tab->setPosition(ofGetWidth() - tab->width, buttonWidth);
	tab->collapsedX = ofGetWidth() - collapsedWidth;
	tab->expandedX = tab->x;

	// this->width  = 10;//Globals::Theme.indexScroller.width;
	IndexScroller* scroller = new IndexScroller();
	scroller->setSize(buttonWidth / 2, cnv->height);

	vector<string> items = { "+-", "UI", "AB" };

	for (char letter = 'Z'; letter >= 'A'; --letter){
		items.insert(items.begin() + 1, string(1, letter));
	}

	scroller->setContent(items);
	scroller->setPosition(tab->width - scroller->width, 0);
	tab->children.push_back(scroller);

	this->children.push_back(tab);
	// object-tab


	// top-bar
	bar = new GuiElement("bar");
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
	bar = new GuiElement("bar");
	bar->set(0, ofGetHeight() - buttonWidth, ofGetWidth(), buttonWidth);
	bar->id = "bottom-bar";

	btn = new Button("edit", Button::TYPE_TOGGLE);
	btn->setPosition(0, 0);
	btn->setSize(bar->height, bar->height);
	bar->children.push_back(btn);

	btn = new Button("settings", Button::TYPE_TOGGLE);
	btn->setPosition(bar->width - bar->height, 0);
	btn->setSize(bar->height, bar->height);
	bar->children.push_back(btn);

	this->children.push_back(bar);
	// bottom-bar


	cnv->set(PdGui::instance().getCanvases()[0]);
}


//--------------------------------------------------------------
void MainWindow::reload(){
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
void MainWindow::onPdEvent (PdEvent&  aPdEvent){

	switch(aPdEvent.type){

		case PdEvent::TYPE_CANVAS_OPEN:
			break;

		case PdEvent::TYPE_CANVAS_EDIT:
			break;

		default:
			break;
	}
}

//--------------------------------------------------------------
// void MainWindow::draw(){

// }

