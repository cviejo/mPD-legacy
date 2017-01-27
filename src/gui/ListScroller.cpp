#include <algorithm> // std::sort
#include "ofMain.h"
#include "ListScroller.h"
// #include "../Globals.h"


using std::sort;


//--------------------------------------------------------------
ListScroller::ListScroller(){

	// _font.load(Globals::DPI * .07f);

	// this->width  = Globals::Theme.objectScroller.width;
	// this->height = ofGetHeight() - Globals::Theme.button.width * 2;
}


//---------------------------VIRTUAL--------------------------//
//--------------------------------------------------------------
void ListScroller::draw(){

	// this->drawItems();

	// ofSetColor(Globals::Theme.objectScroller.header.color.background);
	// ofDrawRectangle(this->x, this->y, this->width, this->header->height);

	// ofSetColor(Globals::Theme.objectScroller.color.front);
	// _font.draw(this->header->content,
				  // this->x + Globals::Theme.objectScroller.item.padding, 
				  // this->y + this->header->contentY - this->header->y);

	// this->drawDraggedItem();
}


//--------------------------------------------------------------
void ListScroller::drawItems(){

	// float offsetY = _draggedY + _scrollValue;

	// ofTranslate(0, this->y - offsetY);

	// for (auto &item : _content){

		// float itemY = item->y - offsetY;

		// if(itemY + item->height >= 0 && itemY < this->height){

			// if (item->isHeader){
				// ofSetColor(Globals::Theme.objectScroller.header.color.background);
			// }
			// else if (this->selection == item){
				// ofSetColor(Globals::Theme.objectScroller.item.color.selection);
			// }
			// else {
				// ofSetColor(Globals::Theme.objectScroller.item.color.background);
			// }
			// ofDrawRectangle(this->x, item->y, this->width, item->height);

			// if (item->isHeader){
				// ofSetColor(Globals::Theme.objectScroller.header.color.front);
			// }
			// else {
				// ofSetColor(Globals::Theme.objectScroller.item.color.front);
			// }
			// _font.draw(item->content,
						  // this->x + Globals::Theme.objectScroller.item.padding,
						  // item->contentY);
		// }
	// }

	// ofTranslate(0, -(this->y - offsetY));
}


//--------------------------------------------------------------
void ListScroller::drawDraggedItem(){

	// if(_dragging && this->selection){

		// ofRectangle rect = _font.getRect(this->selection->content);

		// rect.height = _font.lineHeight * 2;

		// ofSetColor(Globals::Theme.node.color.front);
		// _font.draw(this->selection->content, _draggedLoc.x - rect.width * .5f, _draggedLoc.y - rect.height * 1.5f);

		// rect.width  = rect.height * 4;

		// ofSetColor(Globals::Theme.node.color.front, 130);
		// ofRect(_draggedLoc.x - rect.width * .5f, _draggedLoc.y - rect.height * .5f, rect.width, rect.height);
	// }
}


//--------------------------------------------------------------
void ListScroller::onPressed  (int aX, int aY, int aId){

	// this->selection = NULL;

	// auto offsetPressY = _scrollValue + _pressY - this->y;

	// for (auto &elem : _content){

		// if (!elem->isHeader && elem->inside(1, offsetPressY)){
			// this->selection = elem;
			// break;
		// }
	// }
}


//--------------------------------------------------------------
void ListScroller::onDragged  (int aX, int aY, int aId){

	// object created
	
	// if(aX < this->x){

		// if(!_dragging && this->selection){
			// _dragging = true;
		// }
		// else if(_dragging){
			// _draggedLoc.set(aX, aY);
		// }
	// }
	// else if (_contentHeight > this->height){

		// _dragging = false;
		// _draggedY = _pressY - aY;

		// int offset = _scrollValue + _draggedY;

		// if(offset < 0){
			
			// _scrollValue = 0;
			// _draggedY    = 0;
			// _pressY      = aY;
		// }
		// else if(offset >= _contentHeight - this->height){//_header->height){
			
			// _scrollValue = _contentHeight - this->height;//-_header->height;
			// _draggedY    = 0;
			// _pressY      = aY;
		// }
	// }
	
	// this->setHeader();
}


//--------------------------------------------------------------
void ListScroller::onReleased  (int aX, int aY, int aId){

	// if(_dragging && this->selection){

		// int locX = _draggedLoc.x - _font.lineHeight * 4;
		// int locY = _draggedLoc.y - _font.lineHeight;

		// AppEvent event(AppEvent::TYPE_CREATE_OBJECT, this->selection->content, locX, locY);
		// ofNotifyEvent(AppEvent::events, event);
	// }

	// this->selection = NULL;

	// _scrollValue += _draggedY;

	// _dragging = false;
	// _draggedY = 0;
	
	// this->setHeader();
}


//--------------------------------------------------------------
void ListScroller::onAppEvent(AppEvent& aAppEvent){

	// if(aAppEvent.type == AppEvent::TYPE_SCROLLER_LETTER_CHANGED){

		// for (auto &elem : _content){

			// if (elem->isHeader && ofToUpper(elem->content) == ofToUpper(aAppEvent.message)){

				// _scrollValue = elem->y;

				// if(_scrollValue > _contentHeight - this->height){
					// _scrollValue = _contentHeight - this->height;
				// }
				// break;
			// }
		// }
	// }

	// this->setHeader();
}


//---------------------------PUBLIC---------------------------//
//--------------------------------------------------------------
void ListScroller::setContent(vector<string> aItems){

	// this->clear();

	// string headers[] = {
		// "A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
		// "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
		// "U", "V", "W", "X", "Y", "Z"
	// };

	// for (auto header : headers){
		// _content.push_back(new ScrollerItem(header, true));
	// }

	// for (auto name : aItems){

		// if (this->itemAllowed(name)){
			// _content.push_back(new ScrollerItem(name));
		// }
	// }


	// sort(_content.begin(), _content.end(), compareItems);

	// // these 5 items won't work well with the sorting, 
	// // so we add them later
	// _content.push_back(new ScrollerItem("UI", true));
	// _content.push_back(new ScrollerItem("AB", true));

	// _content.insert(_content.begin(), new ScrollerItem("||"));
	// _content.insert(_content.begin(), new ScrollerItem("|"));
	// _content.insert(_content.begin(), new ScrollerItem("+-", true));

	// this->positionContent();

	// this->header = _content.front();
}

//--------------------------PRIVATE---------------------------//
//--------------------------------------------------------------
void ListScroller::setHeader(){

	// float offset = _draggedY + _scrollValue;

	// for(int i = _content.size() - 1; i >= 0; i--){

		// if(_content[i]->isHeader && _content[i]->y - offset <= 0){
			// this->header = _content[i];
			// break;
		// }
	// }
}


//--------------------------------------------------------------
void ListScroller::positionContent(){

	// _contentHeight = 0;

	// for (auto &item : _content){

		// item->y      = _contentHeight;
		// item->width  = 30;//this->width; // anything bigger than 0, really
		// item->height = item->isHeader ? Globals::Theme.objectScroller.header.height :
												  // Globals::Theme.objectScroller.item.height;

		// item->contentY = item->y + item->height / 2 + _font.lineHeight / 2;

		// _contentHeight += Globals::Theme.objectScroller.padding + item->height;
	// }
}


//--------------------------------------------------------------
bool ListScroller::itemAllowed(string aItemName){

	vector<string> invalid = {
		// "|", "||",
		"scalar define",
		"gatom", "objectmaker", "classmaker","canvas",
		"list inlet", "list append", "list tosymbol",   "list trim",
		"list split", "list length", "list fromsymbol", "list prepend",
		"text define", "text sequence", "text get", "text size",
		"text tolist", "text fromlist", "text set", "text search"
	};

	bool duplicate = false;

	for (auto elem : this->children){

		if (elem->id == aItemName){
			duplicate = true;
		}
	}

	return !ofContains(invalid, aItemName) && !duplicate;
}


//--------------------------------------------------------------
// bool ListScroller::compareItems(const ScrollerItem* a, const ScrollerItem* b){

	// return ofToUpper(a->content) < ofToUpper(b->content);
// }

