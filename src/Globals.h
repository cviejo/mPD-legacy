#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "util/mPDTheme.h"
#include "util/mPDSettings.h"
#include "ofxPd.h"

using namespace pd;

//--------------------------------------------------------------
class Globals{

	public:

		static ApplicationState AppState;
		static string           UserFolder;
		static float            DPI;
		static mPDTheme         Theme;
		static mPDSettings      Settings;
		static ofxPd            Pd;
};
