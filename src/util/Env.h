#pragma once

#include "ofMain.h"


//--------------------------------------------------------------
class Env {

	public:

		static float  getDPI();
		static string getStoragePath();
		static string message(vector<string> aArgs);
};
