#include "ofMain.h"
#include "App.h"
#ifdef TARGET_ANDROID
#include <jni.h>
#endif


//--------------------------------------------------------------
int main( ){

	ofSetupOpenGL(1024, 768, OF_WINDOW);
	// ofSetupOpenGL(100, 100, OF_WINDOW);
	ofRunApp(new App());
}


#ifdef TARGET_ANDROID
//--------------------------------------------------------------
extern "C"{
	void Java_cc_openframeworks_OFAndroid_init( JNIEnv*  env, jobject  thiz ){
		main();
	}
}
#endif

