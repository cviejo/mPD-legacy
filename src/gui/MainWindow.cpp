#include "MainWindow.h"
#include "PdGui.h"
#include "Canvas.h"
#include "SideTab.h"
#include "Button.h"
#include "IndexScroller.h"
#include "ListScroller.h"
// #include "Bar.h"



//--------------------------------------------------------------
MainWindow::MainWindow(){

	ofAddListener(PdEvent::events, this, &MainWindow::onPdEvent);

	this->setPosition(200, 200);
	this->setSize(ofGetWidth() - 400, ofGetHeight() - 400);


	Button* btn;


	//-------top-bar-------
	GuiElement* topBar = new GuiElement("bar");
	topBar->setPosition(0, 0);

	btn = new Button("settings", Button::TYPE_TOGGLE);
	btn->setPosition(this->width - btn->width, 0);
	topBar->addChild(btn);

	this->addChild(topBar);
	//-------top-bar-------


	//-------bottom-bar-------
	GuiElement* bottomBar = new GuiElement("bar");

	btn = new Button("edit", Button::TYPE_TOGGLE);
	btn->setPosition((this->width - btn->width) / 2, 0);
	bottomBar->addChild(btn);

	btn = new Button("menu-tab", Button::TYPE_TOGGLE);
	btn->setPosition(0, 0);
	bottomBar->addChild(btn);

	btn = new Button("object-tab", Button::TYPE_TOGGLE);
	btn->setPosition(this->width - btn->width, 0);
	bottomBar->addChild(btn);

	bottomBar->setPosition(0, this->height - bottomBar->height);
	this->addChild(bottomBar);
	//-------bottom-bar-------


	//------- menu-tab-------
	SideTab* leftTab = new SideTab(SideTab::DOCK_SIDE_LEFT);
	leftTab->id = "menu-tab";
	leftTab->setPosition(0, topBar->getBottom());
	leftTab->setHeight(this->height - topBar->height - bottomBar->height);
	leftTab->expandedX = 0;

	vector<string> zoomBtns = { "zoom-in", "zoom-out", "grid" };

	for (int i = 0; i < zoomBtns.size(); i++) {
		btn = new Button(zoomBtns[i]);
		btn->setPosition(0, btn->height * (i + 1));
		leftTab->addChild(btn);
	}

	vector<string> buttons = { "trash", "undo", "paste", "copy" };

	for (int i = 0; i < buttons.size(); i++) {
		btn = new Button(buttons[i]);
		btn->setPosition(0, leftTab->height - btn->height * (i + 2));
		leftTab->addChild(btn);
	}

	// btn = new Button("grid", Button::TYPE_TOGGLE);
	// btn->setPosition(0, leftTab->height - btn->height * 7);
	// leftTab->addChild(btn);

	this->children.insert(this->children.begin(), leftTab);
	//------- menu-tab-------


	//------- object-tab-------
	SideTab* rightTab = new SideTab(SideTab::DOCK_SIDE_RIGHT);
	rightTab->id = "object-tab";
	rightTab->setHeight(leftTab->height);

	// vector<string> sections = { "+-", "UI", "AB" };
	vector<string> sections = { "+-" };
	for (char letter = 'Z'; letter >= 'A'; --letter){
		sections.insert(sections.begin() + 1, string(1, letter));
	}

	ListScroller* scroller = new ListScroller();
	scroller->setPosition(0, 0);
	scroller->setHeight(leftTab->height);
	scroller->backgroundColor.set(130);
	scroller->setContent(sections, true);
	scroller->setContent(PdGui::instance().getNodeNames(), false);

	ofLogVerbose() << "temp";
	ofLogVerbose() << scroller->width;
	ofLogVerbose() << scroller->height;

	IndexScroller* index = new IndexScroller();
	index->x = rightTab->width;
	index->setPosition(scroller->width, 0);
	index->setHeight(rightTab->height);
	index->setContent(sections);

	rightTab->addChild(scroller);
	rightTab->addChild(index);
	rightTab->setPosition(this->width - rightTab->width, topBar->getBottom());
	rightTab->expandedX = this->width - rightTab->width + 1;

	this->children.insert(this->children.begin(), rightTab);
	//------- object-tab-------


	//-------canvas-------
	Canvas* cnv = new Canvas(this->width, this->height - topBar->height - bottomBar->height);
	cnv->setPosition(leftTab->collapsedX + leftTab->width, topBar->getBottom());
	this->children.insert(this->children.begin(), cnv);
	// //-------canvas-------


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

	// this->drawChildren();
// }

