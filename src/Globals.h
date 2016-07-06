#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "util/mPDTheme.h"
#include "util/mPDSettings.h"
#ifndef WIN32
#include "ofxPd.h"
#else
#include "util/PdMock.h"
#endif

using namespace pd;

//--------------------------------------------------------------
class Globals{

	public:

		static ApplicationState AppState;
		static string           UserFolder;
		static float            DPI;
		static mPDTheme         Theme;
		static mPDSettings      Settings;
#ifndef WIN32
		static ofxPd            Pd;
#else
		static PdMock           Pd;
#endif
};
