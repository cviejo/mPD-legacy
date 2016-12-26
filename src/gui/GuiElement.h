#pragma once

#include "ofMain.h"
// #include "../util/AppEvent.h"


//--------------------------------------------------------------
class GuiElement : public ofRectangle {

	public:

		string      id        = "";
		bool        visible   = true;
		bool        clickable = true;
		bool        pressed   = false;
		int         z         = 0;
		ofPoint     press;

		GuiElement(int aZ = 0);
		~GuiElement(void);

		virtual void update(){};
		virtual void draw(){};
		virtual void init(int aWidth, int aHeight){};
		// virtual void onAppEvent(AppEvent& aAppEvent){};
		virtual void onClick(){};
		virtual void onPressed(int aX, int aY, int aId){};
		virtual void onDragged(int aX, int aY, int aId){};
		virtual void onReleased(int aX, int aY, int aId){};
		virtual void onDoubleClick(int aX, int aY){};
		virtual void onPressCancel(){};
};

