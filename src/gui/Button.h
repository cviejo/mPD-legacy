#pragma once

#include "ofMain.h"
#include "GuiElement.h"
#include "AppEvent.h"


//--------------------------------------------------------------
class Button : public GuiElement {

	public:

		enum ButtonType {
			TYPE_PUSH = 0,
			TYPE_DRAG,
			TYPE_TOGGLE
		};

		ButtonType type    = TYPE_PUSH;
		bool       on      = false;
		bool       enabled = true;
		bool*      togglePointer = NULL;

		Button(string aId = "", ButtonType aType = TYPE_PUSH, bool* aTogglePointer = NULL);

		virtual void draw();
		virtual void onPressed(int aX, int aY, int aId);
		virtual void onDragged(int aX, int aY, int aId);
		virtual void onReleased(int aX, int aY, int aId);
		virtual void onAppEvent(AppEvent& aAppEvent);

	protected:

		ofPoint     _draggedLoc;
		ofImage     _image;
		ofRectangle _imageRect;
};
