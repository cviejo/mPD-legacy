#include "Font.h"
#include "../Globals.h"


vector<ofTrueTypeFont*> Font::Fonts;


//--------------------------------------------------------------
void Font::load(int aSize){

	this->size = aSize;

	// for (auto &font : Font::Fonts){

	// 	if (font->getSize() == this->size){
	// 		_font = font;
	// 		break;
	// 	}
	// }

	// if (_font == NULL){
		_font = new ofTrueTypeFont();
		_font->load(Globals::Theme.font.file, aSize);
		// _font->load("/Developer/OF/v0.9.3/apps/pd/mPD/bin/data/fonts/", aSize);
		// _font->load("/Developer/OF/v0.9.3/apps/pd/mPD/bin/data/fonts/Inconsolata.otf", aSize);
		_font->load("fonts/Inconsolata.otf", aSize);
		// _font->load("DejaVuSansMono.ttf", aSize);
		// _font->load(OF_TTF_SANS, aSize);
		Font::Fonts.push_back(_font);
	// }

	this->lineHeight = _font->getLineHeight();
	this->setCharWidth();
}


//--------------------------------------------------------------
void Font::draw(string aString, int aX, int aY){

	_font->drawString(aString, aX, aY);
}


//--------------------------------------------------------------
int Font::getStringWidth(string aString){

	return this->getRect(aString).width;
}


//--------------------------------------------------------------
ofRectangle Font::getRect(string aString){

	return _font->getStringBoundingBox(aString, 60, 60);
}


//--------------------------------------------------------------
void Font::setCharWidth(){

	string temp = "a";

	for(int i = 0; i < 500; i++){
		temp += "a";
	}

	this->charWidth = floor( (float)this->getStringWidth(temp) / (float)temp.size() + 0.5f);
}
