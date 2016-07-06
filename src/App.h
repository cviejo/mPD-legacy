#pragma once

#include "ofMain.h"
#include "gui/Canvas.h"
#include "gui/Frame.h"
#include "gui/MenuTab.h"
#include "gui/ObjectTab.h"


#ifdef TARGET_ANDROID
#include "ofxAndroid.h"
#include "ofxAndroidUtils.h"
class App : public ofxAndroidApp {
#else
class App : public ofBaseApp {
#endif

	public:

		int  width;
		int  height;

		void setup();
		void init();
		void draw();

		void keyPressed    (int aKey);

		void touchDown     (int aX, int aY, int aId);
		void touchMoved    (int aX, int aY, int aId);
		void touchUp       (int aX, int aY, int aId);
		void touchDoubleTap(int aX, int aY, int aId);
		void touchCancelled(int aX, int aY, int aId);

		void gotMessage    (ofMessage  aMsg);

		void reloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();

		void audioReceived (float * aInput,  int aBufferSize, int aChannelCount);
		void audioRequested(float * aOutput, int aBufferSize, int aChannelCount);

		void osMessage(string aEventType, string aMsg);

#ifdef TARGET_ANDROID
		bool onScaleBegin(ofxAndroidScaleEventArgs& aArgs);
		bool onScale(ofxAndroidScaleEventArgs& aArgs);
		bool onScaleEnd(ofxAndroidScaleEventArgs& aArgs);
#else
		void mouseDragged (int aX, int aY, int aButton) { touchMoved(aX, aY, aButton); }
		void mousePressed (int aX, int aY, int aButton) { touchDown (aX, aY, aButton); }
		void mouseReleased(int aX, int aY, int aButton) { touchUp   (aX, aY, aButton); }
		void mouseScrolled(ofMouseEventArgs & mouse);
#endif

	private:

		Canvas*    _canvas;
		Frame*     _frame;
		ObjectTab* _objectTab;
		MenuTab*   _menuTab;

		ofPoint    _down;
		ofPoint    _up;

		void initAudio();
};

