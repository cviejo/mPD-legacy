#pragma once

#include "ofMain.h"
#include "GuiElement.h"
// #include "PdGui.h"
// #include "../Globals.h"
// #include "../util/Font.h"
// #include "../util/AppEvent.h"
// #include "./canvas-renderers/CanvasRenderer.h"


//--------------------------------------------------------------
class SideTab : public GuiElement {

	public:

		enum TabState {
			STATE_EXPANDED = 0,
			STATE_EXPANDING,
			STATE_COLLAPSED,
			STATE_COLLAPSING
		};

		enum DockSide {

			DOCK_SIDE_LEFT = 0,
			DOCK_SIDE_RIGHT
		};

		TabState state = STATE_COLLAPSED;

		int collapsedX     = 0;
		int expandedX      = 0;
		int collapsedWitdh = 0;

		DockSide side;

		SideTab(DockSide aDockingSide);

		virtual void update();
		virtual void draw();
		virtual void onAppEvent(AppEvent& AppEvent);

	protected:

		int _animationSpeed;

		void clip();
		void updateState();
};

