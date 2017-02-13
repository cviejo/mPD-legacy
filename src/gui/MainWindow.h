#pragma once

#include "GuiElement.h"
#include "PdEvent.h"


//--------------------------------------------------------------
class MainWindow : public GuiElement {

	public:

		MainWindow();

		// virtual void draw();
		void         reload();
		virtual void onAppEvent(AppEvent& aAppEvent);
		void         onPdEvent (PdEvent&  aPdEvent);
};

