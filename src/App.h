#pragma once

#include "ofMain.h"
#include "GuiElement.h"
#if defined(TARGET_ANDROID)
#include "ofxAndroid.h"
#elif defined(TARGET_OF_IOS)
#include "ofxiOS.h"
#include "ofPinchGestureRecognizer.h"
#endif


#if defined(TARGET_ANDROID)
class App : public ofxAndroidApp {
#elif defined(TARGET_OF_IOS)
class App : public ofxiOSApp {
#else
class App : public ofBaseApp {
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

#if defined(TARGET_ANDROID)
		bool onScaleBegin(ofxAndroidScaleEventArgs& aArgs);
		bool onScale(ofxAndroidScaleEventArgs& aArgs);
		bool onScaleEnd(ofxAndroidScaleEventArgs& aArgs);
		void swipe(ofxAndroidSwipeDir swipeDir, int id);
#elif !defined(TARGET_OF_IOS)
		void mouseDragged (int aX, int aY, int aButton) { touchMoved(aX, aY, aButton); }
		void mousePressed (int aX, int aY, int aButton) { touchDown (aX, aY, aButton); }
		void mouseReleased(int aX, int aY, int aButton) { touchUp   (aX, aY, aButton); }
		void mouseScrolled(ofMouseEventArgs & mouse);
#endif

	private:

		bool        _scaling   = false;
		bool        _computing = true;
		ofFbo       _frame;
		GuiElement* _mainWindow;
#if defined(TARGET_OF_IOS)
		ofPinchGestureRecognizer* _pinch;
#endif

		void initAudio();
		void initSearchPaths();
		void initEventListeners();
};

