#pragma once

#include "ofMain.h"


//--------------------------------------------------------------
class ScrollerItem : public ofRectangle {

	public:

		string content;
		int    contentX;
		int    contentY;
		bool   isHeader;

		ScrollerItem (string aContent, bool aIsHeader = false) : content(aContent), isHeader(aIsHeader) {

			this->x      = 0;
			this->y      = 0;
			this->width  = 0;
			this->height = 0;
		}
};
