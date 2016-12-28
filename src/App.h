#pragma once

#include "ofMain.h"
#include "GuiElement.h"
#ifdef TARGET_ANDROID
#include "ofxAndroid.h"
#endif


#ifdef TARGET_ANDROID
class ofApp : public ofxAndroidApp {
#else
class ofApp : public ofBaseApp {
#endif

	public:

		void setup();
		void draw();

		void keyPressed(int key);
		void windowResized(int w, int h);
		void gotMessage(ofMessage msg);
		
		void audioReceived (float * aInput,  int aBufferSize, int aChannelCount);
		void audioRequested(float * aOutput, int aBufferSize, int aChannelCount);

		void touchDown(int x, int y, int id);
		void touchMoved(int x, int y, int id);
		void touchUp(int x, int y, int id);
		void touchDoubleTap(int x, int y, int id);
		void touchCancelled(int x, int y, int id);

		void pause();
		void stop();
		void resume();
		void reloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();

#ifdef TARGET_ANDROID
		bool onScaleBegin(ofxAndroidScaleEventArgs& aArgs);
		bool onScale(ofxAndroidScaleEventArgs& aArgs);
		bool onScaleEnd(ofxAndroidScaleEventArgs& aArgs);
		void swipe(ofxAndroidSwipeDir swipeDir, int id);
#else
		void mouseDragged (int aX, int aY, int aButton) { touchMoved(aX, aY, aButton); }
		void mousePressed (int aX, int aY, int aButton) { touchDown (aX, aY, aButton); }
		void mouseReleased(int aX, int aY, int aButton) { touchUp   (aX, aY, aButton); }
		void mouseScrolled(ofMouseEventArgs & mouse);
#endif

	private:

		vector<GuiElement*> _guiElements;

		void initAudio();
		void initSearchPaths();
		void initEventListeners();
};

