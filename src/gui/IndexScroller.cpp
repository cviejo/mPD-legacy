#include "IndexScroller.h"
// #include "../Globals.h"
#include "AppEvent.h"


//--------------------------------------------------------------
void IndexScroller::draw(){

	// ofSetColor(Globals::Theme.indexScroller.color.background);
	ofSetColor(30, 30, 33);
	ofDrawRectangle(*this);

	// // ofSetColor(Globals::Theme.indexScroller.color.selection);
	ofSetColor(115);
	ofDrawRectangle(this->x, this->y + _scrollY - _itemHeight * .5f, this->width, _itemHeight);

	// ofSetColor(Globals::Theme.indexScroller.color.front);
	ofSetColor(130);

	for (auto& item : _content){
		_font.drawString(item->id, this->x + item->x, this->y + item->y);
	}
}


//--------------------------------------------------------------
void IndexScroller::onPressed(int aX, int aY, int aId){ this->updateScroll(aX, aY); }


//--------------------------------------------------------------
void IndexScroller::onDragged(int aX, int aY, int aId){ this->updateScroll(aX, aY); }


//--------------------------------------------------------------
void IndexScroller::onReleased  (int aX, int aY, int aId){

	this->updateScroll(aX, (int)(this->selection * _itemHeight + _itemHeight * 0.5f));
}


//--------------------------------------------------------------
void IndexScroller::updateScroll(int aX, int aY) {

	_scrollY = aY - this->y;

	auto limit = _itemHeight * 0.5f;
	auto count = (float)_content.size();

	if (_scrollY < limit) {
		_scrollY = limit;
	}
	else if ( _scrollY > this->height - limit) {
		_scrollY = this->height - limit;
	}

	int selectionIndex = (int)(count / this->height * _scrollY);

	if (selectionIndex > count - 1){
		selectionIndex = count - 1;
	}

	if (this->selection != selectionIndex){

		this->selection = selectionIndex;

		AppEvent event(AppEvent::TYPE_SCROLLER_LETTER_CHANGED, _content[this->selection]->id);

		ofNotifyEvent(AppEvent::events, event);
	}
}

//--------------------------PRIVATE---------------------------//
//--------------------------------------------------------------
void IndexScroller::setContent(vector<string> aContent){

	// TODO: delete previous content?

	_itemHeight = (float)this->height / (float)aContent.size();

	float fontHeight = _itemHeight * .5f;

	// TODO: set some maxHeight based on DPI
	// float maxHeight  = (int)(0.07f * Globals::DPI);

	// if (fontHeight > maxHeight){
		// fontHeight = maxHeight;
	// }

	_font.load("fonts/DejaVuSansMono.ttf", fontHeight, true, true);
	_font.setLineHeight(_itemHeight);

	for (auto& contentItem : aContent){

		auto item       = new GuiElement();
		auto itemY      = _itemHeight + _content.size() * _itemHeight;
		auto itemSize   = _font.getStringBoundingBox(contentItem, 0, 0);
		auto totalWidth = _font.getStringBoundingBox(contentItem + "-", 0, 0).width;

		item->id = contentItem;
		item->y  = itemY - (_itemHeight - itemSize.height) / 2;

		item->setSize(itemSize.width, itemSize.height);

		_content.push_back(item);

		if (this->width < totalWidth){
			this->width = totalWidth;
		}
	}

	for (auto& item : _content){
		item->x = (this->width - item->width) * .5f;
	}

	this->selection = _content.size() - 1;

	this->onPressed (0, 0, 0);
	this->onDragged (0, 0, 0);
	this->onReleased(0, 0, 0);
}

