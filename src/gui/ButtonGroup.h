#pragma once

#include "ofMain.h"
#include "GUIElement.h"
#include "Button.h"


//--------------------------------------------------------------
class ButtonGroup : public GUIElement
{
	public:

		Orientation     orientation = ORIENTATION_VERTICAL;
		vector<Button*> buttons;

		virtual void update();

		void add(Button* aBtn);
		void setVisibility(bool aValue);
};
