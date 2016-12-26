
#include "PdGuiMessage.h"


//--------------------------------------------------------------
PdGuiMessage::PdGuiMessage(string aMsg){

	auto message = aMsg;
	
	ofStringReplace(message, "\x1f", "");

	this->command = ofSplitString(message, " ")[0];

	auto parts = ofSplitString(message, (this->command + " "));

	if (parts.size()){
		this->args = ofSplitString(parts[1], ",");
	}

	this->parseIds();
}


//--------------------------------------------------------------
void PdGuiMessage::parseIds(){

	if (this->args.size() && this->args[0].find("\"") == 0){
		this->canvasId = this->unquote(this->args[0]);
	}

	if (this->args.size() > 1 && this->args[1].find("\"") == 0){
		this->nodeId = this->unquote(this->args[1]);
	}
}


//--------------------------------------------------------------
void PdGuiMessage::parseRect(int index){

	this->x = ofToInt(this->args[index]);
	this->y = ofToInt(this->args[index + 1]);

	this->setSize(ofToInt(this->args[index + 2]) - this->x,
	              ofToInt(this->args[index + 3]) - this->y);
}


//--------------------------------------------------------------
void PdGuiMessage::parseColor(int startIndex){

	auto colorString = this->unquote(this->args[startIndex]);

	ofStringReplace(colorString, "none", "#FFFFFF");
	ofStringReplace(colorString, "#",    "");

	this->color = ofHexToInt(colorString);
}


//--------------------------------------------------------------
string PdGuiMessage::unquote(string& str){

	int front   = str.front() == '"';
	int back    = str.back()  == '"';

	return str.substr(front, str.length() - front - back);
}

