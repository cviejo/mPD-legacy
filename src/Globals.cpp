#include "Globals.h"
#include "util/Env.h"

ApplicationState Globals::AppState   = APP_STATE_START;
string           Globals::UserFolder = ofFilePath::getCurrentWorkingDirectory();
float            Globals::DPI        = 350;
mPDTheme         Globals::Theme;
mPDSettings      Globals::Settings;
#ifndef WIN32
ofxPd            Globals::Pd;
#else
PdMock           Globals::Pd;
#endif
