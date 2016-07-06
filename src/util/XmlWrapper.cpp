#include "XmlWrapper.h"



//--------------------------------------------------------------
ofColor XmlWrapper::getColor(string aPath){

	int alpha = this->xml.getAttribute(aPath, "a", 255);

	if (this->xml.attributeExists(aPath, "grey")){

		int grey = this->xml.getAttribute(aPath, "grey", 255);

		return ofColor(grey, alpha);
	}
	else {

		int r = this->xml.getAttribute(aPath, "r", 255);
		int g = this->xml.getAttribute(aPath, "g", 255);
		int b = this->xml.getAttribute(aPath, "b", 255);

		return ofColor(r, g, b, alpha);
	}
}


//--------------------------------------------------------------
float XmlWrapper::getFloat(string aPath){

	return this->xml.getValue(aPath, 0.0f);
}


//--------------------------------------------------------------
double XmlWrapper::getDouble(string aPath){

	return this->xml.getValue(aPath, 0.0);
}


//--------------------------------------------------------------
string XmlWrapper::getString(string aPath){

	return this->xml.getValue(aPath, "");
}


//--------------------------------------------------------------
bool XmlWrapper::getBool(string aPath){

	return this->getString(aPath) == "true";
}


//--------------------------------------------------------------
void XmlWrapper::loadFile(string aPath){

	this->path = aPath;

	this->xml.loadFile(aPath);
}


//--------------------------------------------------------------
void XmlWrapper::saveFile(){

	this->xml.save(this->path);
}
