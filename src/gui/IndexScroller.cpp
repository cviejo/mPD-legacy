#include "IndexScroller.h"
#include "../Globals.h"
#include "../util/AppEvent.h"


//---------------------------VIRTUAL--------------------------//
//--------------------------------------------------------------
IndexScroller::IndexScroller() : GUIElement(4){

	this->width  = Globals::Theme.indexScroller.width;
	this->height = ofGetHeight() - Globals::Theme.button.width * 2;

	this->initContent();
	this->updateScroll(0, 0);
}


//--------------------------------------------------------------
void IndexScroller::draw(){

	ofSetColor(Globals::Theme.indexScroller.color.background);
	ofDrawRectangle(this->x, this->y, this->width, this->height);

	ofSetColor(Globals::Theme.indexScroller.color.selection);
	ofDrawRectangle(this->x, this->y + _scrollY - _itemHeight * .5, this->width, _itemHeight * 1.2);

	ofSetColor(Globals::Theme.indexScroller.color.front);

	for (auto &item : _content){

		_font.draw(item->content,
		           this->x + item->contentX,
		           this->y + item->y - item->contentY);
	}
}


//--------------------------------------------------------------
void IndexScroller::onPressed  (int aX, int aY, int aId){

	this->updateScroll(aX, aY);
}


//--------------------------------------------------------------
void IndexScroller::onDragged  (int aX, int aY, int aId){

	this->updateScroll(aX, aY);
}


//--------------------------------------------------------------
void IndexScroller::onReleased  (int aX, int aY, int aId){

	int selectionY = (int)(this->selection->y - _itemHeight * .5);

	this->updateScroll(aX, (int)(selectionY + this->y));
}

//---------------------------PUBLIC---------------------------//
//--------------------------------------------------------------
void IndexScroller::reset(){

	this->updateScroll(0,0);

	AppEvent event(AppEvent::TYPE_SCROLLER_LETTER_CHANGED, this->selection->content);
	ofNotifyEvent(AppEvent::events, event);
}


//--------------------------------------------------------------
void IndexScroller::updateScroll(int aX, int aY) {

	_scrollY = aY - this->y;

	auto limit = _itemHeight * 0.5f;

	if (_scrollY < limit) {
		_scrollY = limit;
	}
	else if ( _scrollY > this->height - limit) {
		_scrollY = this->height - limit;
	}

	float itemCount      = (float)_content.size();
	int   selectionIndex = (int)(itemCount / this->height * _scrollY);

	if (selectionIndex > itemCount - 1){
		selectionIndex = itemCount - 1;
	}

	if (this->selection != _content[selectionIndex]){

		this->selection = _content[selectionIndex];

		AppEvent event(AppEvent::TYPE_SCROLLER_LETTER_CHANGED, this->selection->content);
		ofNotifyEvent(AppEvent::events, event);
	}
}

//--------------------------PRIVATE---------------------------//
//--------------------------------------------------------------
void IndexScroller::initContent(){

	string items[] = {
		"+-",
		"A", "B", "C", "D", "E", "F", "G", "H", "I", 
		"J", "K", "L", "M", "N", "O", "P", "Q", "R",
		"S", "T", "U", "V", "W", "X", "Y", "Z",
		"UI", "AB"
	};

	_itemHeight = (float)this->height / 29.0f;

	float fontHeight = _itemHeight * .6f;
	float maxHeight  = (int)(0.07f * Globals::DPI);

	if (fontHeight > maxHeight){
		fontHeight = maxHeight;
	}

	float itemOffsetY = (_itemHeight - fontHeight) / 3;

	_font.load(fontHeight);

	for (auto header : items){

		ScrollerItem* item = new ScrollerItem(header);

		item->width    = item->content.size() * _font.charWidth;// _font.getStringBoundingBox(item->content, 60, 60).width;

		item->y        = _itemHeight + _content.size() * _itemHeight;
		item->contentX = (this->width - item->width) * .5f;
		item->contentY = itemOffsetY;

		_content.push_back(item);
	}
}
