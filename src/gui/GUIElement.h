#pragma once

#include "ofMain.h"
#include "../Constants.h"
#include "../util/AppEvent.h"


//--------------------------------------------------------------
class GUIElement : public ofRectangle {

	public:

		static vector<GUIElement*> All;

		string      id        = "";
		bool        visible   = true;
		bool        clickable = true;
		bool        pressed   = false;
		int         z         = 0;
		ofPoint     press;

		GUIElement(int aZ = 0);
		~GUIElement(void);

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

		void drawBounds();

	private:

		void registerElement  (GUIElement* aElem);
		void unregisterElement(GUIElement* aElem);
};
