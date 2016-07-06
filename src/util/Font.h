#pragma once


#include "ofMain.h"


//--------------------------------------------------------------
class Font {

	public:

		int lineHeight = 0;
		int charWidth  = 0;
		int size       = 0;

		void   load(int aSize);
		void   draw(string aString, int aX, int aY);
		int    getStringWidth(string aString);
		ofRectangle getRect(string aString);

	private:

		static vector<ofTrueTypeFont*> Fonts;

		ofTrueTypeFont* _font = NULL;

		void setCharWidth();
};
