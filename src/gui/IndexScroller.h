#pragma once

#include "ofMain.h"
#include "GuiElement.h"
// #include "ScrollerItem.h"
// #include "../util/Font.h"


//--------------------------------------------------------------
class IndexScroller : public GuiElement {

	public:

		// ScrollerItem* selection;
		int selection;

		void setContent(vector<string> aContent);
		void setLetter(string aLetter);

		virtual void draw();
		virtual void onPressed(int aX, int aY, int aId);
		virtual void onDragged(int aX, int aY, int aId);
		virtual void onReleased(int aX, int aY, int aId);

	private:

		ofTrueTypeFont     _font;
		vector<GuiElement*> _content;
		int                _scrollY;
		float              _itemHeight;
		// vector<ScrollerItem*> _content;
		// vector<string> _content;
		// mFont                  _font;

		void updateScroll(int aX, int aY);
};

