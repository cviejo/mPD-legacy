#pragma once

#include "ofMain.h"
#ifdef TARGET_ANDROID
#include "ofxAndroid.h"
#endif


#ifdef TARGET_ANDROID
class ofApp : public ofxAndroidApp {
#else
class ofApp : public ofBaseApp {
#endif

	public:

		vector <string> gui_items;

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mousePressed(int x, int y, int button);
		void mouseDragged(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
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
		// void mouseDragged (int aX, int aY, int aButton) { touchMoved(aX, aY, aButton); }
		// void mousePressed (int aX, int aY, int aButton) { touchDown (aX, aY, aButton); }
		// void mouseReleased(int aX, int aY, int aButton) { touchUp   (aX, aY, aButton); }
		void mouseScrolled(ofMouseEventArgs & mouse);
#endif

	private:

		void initAudio();
};

