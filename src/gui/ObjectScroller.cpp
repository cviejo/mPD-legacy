#include <algorithm> // std::sort
#include "ofMain.h"
#include "ObjectScroller.h"
#include "../Globals.h"


using std::sort;


//--------------------------------------------------------------
ObjectScroller::ObjectScroller() : GUIElement(3){

	_font.load(Globals::DPI * .07f);

	// string items[] = {
	// 	" ",
	// 	"A", "B", "C", "D", "E", "F", "G", "H", "I"
	// };

	// for (auto item : items){
	// 	string temp = item;
	// 	for(int i=0; i<500; i++){
	// 		temp += item;
	// 	}
	// 	this->itemAllowed(temp);
	// }

	this->width  = Globals::Theme.objectScroller.width;
	this->height = ofGetHeight() - Globals::Theme.button.width * 2;
}


//---------------------------VIRTUAL--------------------------//
//--------------------------------------------------------------
void ObjectScroller::draw(){

	this->drawItems();

	ofSetColor(Globals::Theme.objectScroller.header.color.background);
	ofDrawRectangle(this->x, this->y, this->width, this->header->height);

	ofSetColor(Globals::Theme.objectScroller.color.front);
	_font.draw(this->header->content,
	           this->x + Globals::Theme.objectScroller.item.padding, 
	           this->y + this->header->contentY - this->header->y);

	this->drawDraggedItem();
}


//--------------------------------------------------------------
void ObjectScroller::drawItems(){

	float offsetY = _draggedY + _scrollValue;

	ofTranslate(0, this->y - offsetY);

	for (auto &item : _content){

		float itemY = item->y - offsetY;

		if(itemY + item->height >= 0 && itemY < this->height){

			if (item->isHeader){
				ofSetColor(Globals::Theme.objectScroller.header.color.background);
			}
			else if (this->selection == item){
				ofSetColor(Globals::Theme.objectScroller.item.color.selection);
			}
			else {
				ofSetColor(Globals::Theme.objectScroller.item.color.background);
			}
			ofDrawRectangle(this->x, item->y, this->width, item->height);

			if (item->isHeader){
				ofSetColor(Globals::Theme.objectScroller.header.color.front);
			}
			else {
				ofSetColor(Globals::Theme.objectScroller.item.color.front);
			}
			_font.draw(item->content,
			           this->x + Globals::Theme.objectScroller.item.padding,
			           item->contentY);
		}
	}

	ofTranslate(0, -(this->y - offsetY));
}


//--------------------------------------------------------------
void ObjectScroller::drawDraggedItem(){

	if(_dragging && this->selection){

		ofRectangle rect = _font.getRect(this->selection->content);

		rect.height = _font.lineHeight * 2;

		ofSetColor(Globals::Theme.node.color.front);
		_font.draw(this->selection->content, _draggedLoc.x - rect.width * .5f, _draggedLoc.y - rect.height * 1.5f);

		rect.width  = rect.height * 4;

		ofSetColor(Globals::Theme.node.color.front, 130);
		ofRect(_draggedLoc.x - rect.width * .5f, _draggedLoc.y - rect.height * .5f, rect.width, rect.height);
	}
}


//--------------------------------------------------------------
void ObjectScroller::onPressed  (int aX, int aY, int aId){

	_pressY = aY;

	this->selection = NULL;

	auto offsetPressY = _scrollValue + _pressY - this->y;

	for (auto &elem : _content){

		if (!elem->isHeader && elem->inside(1, offsetPressY)){
			this->selection = elem;
			break;
		}
	}
}


//--------------------------------------------------------------
void ObjectScroller::onDragged  (int aX, int aY, int aId){

	// object created
	if(aX < this->x){

		if(!_dragging && this->selection){
			_dragging = true;
		}
		else if(_dragging){
			_draggedLoc.set(aX, aY);
		}
	}
	else if (_contentHeight > this->height){

		_dragging = false;
		_draggedY = _pressY - aY;

		int offset = _scrollValue + _draggedY;

		if(offset < 0){
			
			_scrollValue = 0;
			_draggedY    = 0;
			_pressY      = aY;
		}
		else if(offset >= _contentHeight - this->height){//_header->height){
			
			_scrollValue = _contentHeight - this->height;//-_header->height;
			_draggedY    = 0;
			_pressY      = aY;
		}
	}
	
	this->setHeader();
}


//--------------------------------------------------------------
void ObjectScroller::onReleased  (int aX, int aY, int aId){

	if(_dragging && this->selection){

		int locX = _draggedLoc.x - _font.lineHeight * 4;
		int locY = _draggedLoc.y - _font.lineHeight;

		AppEvent event(AppEvent::TYPE_CREATE_OBJECT, this->selection->content, locX, locY);
		ofNotifyEvent(AppEvent::events, event);
	}

	this->selection = NULL;

	_scrollValue += _draggedY;

	_dragging = false;
	_draggedY = 0;
	
	this->setHeader();
}


//--------------------------------------------------------------
void ObjectScroller::onAppEvent(AppEvent& aAppEvent){

	if(aAppEvent.type == AppEvent::TYPE_SCROLLER_LETTER_CHANGED){

		for (auto &elem : _content){

			if (elem->isHeader && ofToUpper(elem->content) == ofToUpper(aAppEvent.message)){

				_scrollValue = elem->y;

				if(_scrollValue > _contentHeight - this->height){
					_scrollValue = _contentHeight - this->height;
				}
				break;
			}
		}
	}

	this->setHeader();
}


//---------------------------PUBLIC---------------------------//
//--------------------------------------------------------------
void ObjectScroller::setContent(vector<string> aItems){

	this->clear();

	string headers[] = {
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
		"K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
		"U", "V", "W", "X", "Y", "Z"
	};

	for (auto header : headers){
		_content.push_back(new ScrollerItem(header, true));
	}

	for (auto name : aItems){
		if (this->itemAllowed(name)){
			_content.push_back(new ScrollerItem(name));
		}
	}


	sort(_content.begin(), _content.end(), compareItems);

	// these 5 items won't work well with the sorting, 
	// so we add them later
	_content.push_back(new ScrollerItem("UI", true));
	_content.push_back(new ScrollerItem("AB", true));

	_content.insert(_content.begin(), new ScrollerItem("||"));
	_content.insert(_content.begin(), new ScrollerItem("|"));
	_content.insert(_content.begin(), new ScrollerItem("+-", true));

	this->positionContent();

	this->header = _content.front();
}


//--------------------------------------------------------------
void ObjectScroller::clear(){

	for (auto &elem : _content){
		delete elem;
	}

	_content.clear();
}

//--------------------------PRIVATE---------------------------//
//--------------------------------------------------------------
void ObjectScroller::setHeader(){

	float offset = _draggedY + _scrollValue;

	for(int i = _content.size() - 1; i >= 0; i--){

		if(_content[i]->isHeader && _content[i]->y - offset <= 0){
			this->header = _content[i];
			break;
		}
	}
}


//--------------------------------------------------------------
void ObjectScroller::positionContent(){

	_contentHeight = 0;

	for (auto &item : _content){

		item->y      = _contentHeight;
		item->width  = 30;//this->width; // anything bigger than 0, really
		item->height = item->isHeader ? Globals::Theme.objectScroller.header.height :
		                                Globals::Theme.objectScroller.item.height;

		item->contentY = item->y + item->height / 2 + _font.lineHeight / 2;

		_contentHeight += Globals::Theme.objectScroller.padding + item->height;
	}
}


//--------------------------------------------------------------
bool ObjectScroller::itemAllowed(string aItemName){

	static const string arr[] = {
		"|", 
		"||", 
		"gatom", 
		"objectmaker", 
		"classmaker", 
		"canvas",
		"list inlet",
		"list append",
		"list prepend",
		"list split",
		"list trim",
		"list length",
		"list fromsymbol",
		"list tosymbol",
		"scalar define",
		"text define",
		"text get",
		"text set",
		"text size",
		"text tolist",
		"text fromlist",
		"text search",
		"text sequence"
	};
	static const vector<string> invalidNames (arr, arr + sizeof(arr) / sizeof(arr[0]));

	bool duplicate = false;

	for (auto elem : _content){

		if (elem->content == aItemName){
			duplicate = true;
		}
	}

	return !ofContains(invalidNames, aItemName) && !duplicate;
}


//--------------------------------------------------------------
bool ObjectScroller::compareItems(const ScrollerItem* a, const ScrollerItem* b){

	return ofToUpper(a->content) < ofToUpper(b->content);
}
