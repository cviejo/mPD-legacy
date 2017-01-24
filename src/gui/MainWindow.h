#pragma once

#include "GuiElement.h"


//--------------------------------------------------------------
class MainWindow : public GuiElement {

	public:

		MainWindow();

		virtual void onAppEvent(AppEvent& aAppEvent);
};

