#include "JsonTheme.h"


//--------------------------------------------------------------
bool JsonTheme::load(string aPath){

	ofBuffer     buffer = ofBufferFromFile(aPath);
	Json::Reader reader;

	return reader.parse(buffer.getText(), *this);
}


//--------------------------------------------------------------
float JsonTheme::getScaledValue(string aType, string aName){

	auto value = (*this)[aType][aName].asInt();

	// TODO: real dpi
#ifdef TARGET_ANDROID
	return 440.0f / 100.0f * value;
#else
	return 150.0f / 100.0f * value;
#endif
}


//--------------------------------------------------------------
int JsonTheme::getHex(string aType, string aName){

	auto value = (*this)[aType][aName].asString();

	ofStringReplace(value, "#", "0x");

	return ofHexToInt(value);
}

