#pragma once

#include "ofMain.h"
#include "AppEvent.h"


//--------------------------------------------------------------
class GuiElement : public ofRectangle {

	public:

		GuiElement();

		string              id        = "";
		bool                visible   = true;
		bool                clickable = true;
		bool                pressed   = false;
		ofPoint             pressedPoint;
		vector<GuiElement*> children;

		virtual void        draw();
		virtual void        update(){};
		virtual void        init(int aWidth, int aHeight){};
		virtual void        onAppEvent(AppEvent& aAppEvent){};
		virtual void        onClick(){};
		virtual void        onPressed(int aX, int aY, int aId){};
		virtual void        onPressed(ofPoint& aLoc){};
		virtual void        onDragged(int aX, int aY, int aId){};
		virtual void        onDragged(ofPoint& aLoc){};
		virtual void        onReleased(int aX, int aY, int aId){};
		virtual void        onReleased(ofPoint& aLoc){};
		virtual void        onDoubleClick(ofPoint& aLoc){};
		virtual void        onPressCancel(){};

		void                drawChildren();
		// void                updatePosition()
		bool                touchDown (ofPoint aLoc);
		void                touchUp   (ofPoint aLoc);
		void                touchMoved(ofPoint aLoc);
		bool                touchTest (ofPoint aLoc);
};

