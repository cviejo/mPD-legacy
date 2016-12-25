#include "ofMain.h"
#include "ofApp.h"
#ifdef TARGET_ANDROID
#include <jni.h>
#endif


//--------------------------------------------------------------
int main( ){

	ofSetupOpenGL(1024, 768, OF_WINDOW);
	ofRunApp(new ofApp());
}


#ifdef TARGET_ANDROID
//--------------------------------------------------------------
extern "C"{
	void Java_cc_openframeworks_OFAndroid_init( JNIEnv*  env, jobject  thiz ){
		main();
	}
}
#endif

