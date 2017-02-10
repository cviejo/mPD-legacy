#pragma once

#include "ofMain.h"
#include "AppEvent.h"
#include "JsonTheme.h"


//--------------------------------------------------------------
class GuiElement : public ofRectangle {

	public:

		static JsonTheme   Theme;

		GuiElement(string aType = "");
		~GuiElement();

		string              id        = "";
		string              type      = "";
		string              text      = "";
		bool                visible   = true;
		bool                clickable = true;
		bool                pressed   = false;
		ofPoint             pressedPosition;
		ofPoint             draggedPosition;
		ofPoint             textPosition;
		vector<GuiElement*> children;
		ofColor             frontColor;
		ofColor             backgroundColor;
		ofColor             selectionColor;

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

		void                clear();
		void                addChild(GuiElement* aChild);
		void                drawChildren();
		void                drawBackground();
		bool                touchDown (ofPoint aLoc);
		void                touchUp   (ofPoint aLoc);
		void                touchMoved(ofPoint aLoc);
		bool                touchTest (ofPoint aLoc);
};

