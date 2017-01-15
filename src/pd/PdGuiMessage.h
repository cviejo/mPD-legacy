#pragma once


#include "ofMain.h"


// class PdGuiMessage : public ofRectangle {
class PdGuiMessage {

	public:

		string message;
		vector<string> args;
		string command;
		string canvasId;
		string nodeId;
		int    color;

		PdGuiMessage(string aMsg);

		void        parseIds();
		ofRectangle parseRect(int index);
		int         parseColor(int index);
		string      unquote(string& str);
};

