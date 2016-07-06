#pragma once

#include "ofMain.h"
#include "GUIElement.h"
#include "ScrollerItem.h"
#include "../util/Font.h"


//--------------------------------------------------------------
class IndexScroller : public GUIElement {

	public:

		ScrollerItem* selection;

		IndexScroller();

		void reset();
		void setLetter(string aLetter);

		virtual void draw();
		virtual void onPressed (int aX, int aY, int aId);
		virtual void onDragged (int aX, int aY, int aId);
		virtual void onReleased(int aX, int aY, int aId);

	private:

		vector<ScrollerItem*> _content;
		int                   _scrollY;
		Font                  _font;
		float                 _itemHeight;

		void initContent();
		void updateScroll(int aX, int aY);
};
