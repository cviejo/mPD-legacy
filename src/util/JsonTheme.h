#pragma once

#include "ofMain.h"
#include "json.h"


//--------------------------------------------------------------
class JsonTheme : public Json::Value {

	public:

		bool  load(string aPath);
		float getScaledValue(string aType, string aName);
		int   getHex(string aType, string aName);
};

