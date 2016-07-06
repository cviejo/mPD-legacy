#pragma once

#include "ofMain.h"
#include "GUIElement.h"
#include "ScrollerItem.h"
#include "../util/Font.h"
#include "../util/AppEvent.h"


//--------------------------------------------------------------
class ObjectScroller : public GUIElement {

	public:

		ScrollerItem* header    = NULL;
		ScrollerItem* selection = NULL;

		ObjectScroller();

		virtual void draw();
		virtual void onPressed(int aX, int aY, int aId);
		virtual void onDragged(int aX, int aY, int aId);
		virtual void onReleased(int aX, int aY, int aId);
		virtual void onAppEvent(AppEvent& aAppEvent);

		void setContent(vector<string> aItems);
		void clear();

	private:

		bool  _dragging      = false;
		float _draggedY      = 0;
		float _pressY        = 0;
		float _scrollValue   = 0;
		int   _contentHeight = 0;

		Font                  _font;
		vector<ScrollerItem*> _content;
		ofPoint               _draggedLoc;

		static bool compareItems(const ScrollerItem* a, const ScrollerItem* b);

		void setHeader();
		void positionContent();
		void drawItems();
		void drawDraggedItem();
		bool itemAllowed(string aItemName);
};
