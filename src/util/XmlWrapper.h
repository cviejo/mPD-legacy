#pragma once


#include "ofMain.h"
#include "ofxXmlSettings.h"


//--------------------------------------------------------------
class XmlWrapper {

	public:

		ofxXmlSettings xml;
		string         path;

		void    loadFile (string aPath);
		void    saveFile ();
		ofColor getColor (string aPath);
		float   getFloat (string aPath);
		double  getDouble(string aPath);
		string  getString(string aPath);
		bool    getBool  (string aPath);
};
