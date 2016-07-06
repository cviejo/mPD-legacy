#include "GUIElement.h"


vector<GUIElement*> GUIElement::All;


//--------------------------------------------------------------
GUIElement::GUIElement(int aZ) : z(aZ){

	this->width  = 0;
	this->height = 0;

	this->registerElement(this);

	ofAddListener(AppEvent::events, this, &GUIElement::onAppEvent);
}


//--------------------------------------------------------------
GUIElement::~GUIElement(void){

	this->unregisterElement(this);
}


//--------------------------------------------------------------
void GUIElement::registerElement(GUIElement* aElem){

	if (!ofContains(GUIElement::All, aElem)){

		for(int i = 0; i < GUIElement::All.size(); i++){

			if(GUIElement::All[i]->z >= aElem->z){
				GUIElement::All.insert(GUIElement::All.begin() + i, this);
				return;
			}
		}

		GUIElement::All.push_back(this);
	}
}


//--------------------------------------------------------------
void GUIElement::unregisterElement(GUIElement* aElem){

	for(int i = 0; i < GUIElement::All.size(); i++){

		if(GUIElement::All[i] == aElem){
			GUIElement::All.erase(GUIElement::All.begin() + i);
			break;
		}
	}
}



//--------------------------------------------------------------
void GUIElement::drawBounds(){

	ofSetColor(255);
	ofFill();
	ofDrawRectangle(this->x, this->y, this->width, this->height);
}
