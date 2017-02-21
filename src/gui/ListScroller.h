#pragma once

#include "ofMain.h"
#include "GuiElement.h"
#include "AppEvent.h"


//--------------------------------------------------------------
class ListScroller : public GuiElement {

	public:

		GuiElement*    selection = NULL;

		ListScroller();

		virtual void   draw();
		virtual void   update();
		virtual void   onAppEvent(AppEvent& aAppEvent);
		virtual void   onPressed (int aX, int aY, int aId);
		virtual void   onDragged (int aX, int aY, int aId);
		virtual void   onReleased(int aX, int aY, int aId);

		void           setContent(vector<string> aItems, bool aHeaderItems);


	private:

		static bool    itemSort(const GuiElement* a, const GuiElement* b);

		GuiElement*    _selection     = NULL;
		int            _draggedY      = 0;
		int            _offsetY       = 0;
		int            _contentHeight = 0;
		bool           _previewActive = false;
		ofRectangle    _previewRect;
		ofTrueTypeFont _font;
		int            _fontHeight;

		void           clip();
		void           drawPreview();
};
















/*
		// ScrollerItem* header    = NULL;
		// ScrollerItem* selection = NULL;

		ListScroller();

		virtual void draw();
		virtual void onPressed(int aX, int aY, int aId);
		virtual void onDragged(int aX, int aY, int aId);
		virtual void onReleased(int aX, int aY, int aId);
		virtual void onAppEvent(AppEvent& aAppEvent);

		void setContent(vector<string> aItems);
		void setHeaders(vector<string> aItems);


	private:

		bool  _dragging      = false;
		float _draggedY      = 0;
		float _pressY        = 0;
		float _scrollValue   = 0;
		int   _contentHeight = 0;

		ofTrueTypeFont     _font;
		// vector<ScrollerItem*> _content;
		ofPoint               _draggedLoc;

		static bool compareItems(const GuiElement* a, const GuiElement* b);

		void setHeader();
		void positionContent();
		void drawItems();
		void drawDraggedItem();
		bool itemAllowed(string aItemName);

*/
