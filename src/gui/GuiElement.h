#pragma once

#include "ofMain.h"
#include "AppEvent.h"


//--------------------------------------------------------------
class GuiElement : public ofRectangle {

	public:

		GuiElement(){
			ofAddListener(AppEvent::events, this, &GuiElement::onAppEvent);
		}

		string      id        = "";
		bool        visible   = true;
		bool        clickable = true;
		bool        pressed   = false;
		ofPoint     pressedPoint;

		virtual void update(){};
		virtual void draw(){};
		virtual void init(int aWidth, int aHeight){};
		virtual void onAppEvent(AppEvent& aAppEvent){};
		virtual void onClick(){};
		virtual void onPressed(int aX, int aY, int aId){};
		virtual void onDragged(int aX, int aY, int aId){};
		virtual void onReleased(int aX, int aY, int aId){};
		virtual void onDoubleClick(int aX, int aY){};
		virtual void onPressCancel(){};
};

