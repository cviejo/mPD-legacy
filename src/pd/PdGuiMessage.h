#pragma once


#include "ofMain.h"


class PdGuiMessage : public ofRectangle {

	public:

		string message;
		vector<string> args;
		string command;
		string canvasId;
		string nodeId;
		int    color;


		PdGuiMessage(string aMsg);
		void parseIds();
		void parseRect(int index);
		void parseColor(int index);
		string unquote(string& str);
};
