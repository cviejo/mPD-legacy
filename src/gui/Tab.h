#pragma once


#include "GUIElement.h"
#include "../util/AppEvent.h"


//--------------------------------------------------------------
class Tab : public GUIElement {

	public:

		enum TabState {
			STATE_EXPANDED = 0,
			STATE_EXPANDING,
			STATE_COLLAPSED,
			STATE_COLLAPSING
		};

		TabState state = STATE_COLLAPSED;

		int collapsedX     = 0;
		int expandedX      = 0;
		int collapsedWitdh = 0;

		DockSide side;

		Tab(DockSide aDockingSide, int aZ) : side(aDockingSide), GUIElement(aZ){}

		virtual void draw();
		virtual void onAppEvent(AppEvent& AppEvent);

	protected:

		int _animationSpeed;

		void clip();
		void updateState();
};
