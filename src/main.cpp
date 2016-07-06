#include "ofMain.h"
#include "App.h"

#ifdef TARGET_ANDROID
#include <jni.h>
#include "ofAppAndroidWindow.h"
#endif


//--------------------------------------------------------------
int main( ){

	ofLogVerbose("OF Start.");

#ifdef TARGET_ANDROID
	ofSetupOpenGL(new ofAppAndroidWindow, 1024,768, OF_WINDOW);
#else
	ofSetupOpenGL(500, 720, OF_WINDOW);
#endif

	ofRunApp(new App);

	return 0;
}



#ifdef TARGET_ANDROID
//--------------------------------------------------------------
void javaMessage(JNIEnv* env, jobject obj, jstring aEvent, jstring aMessage){

	jboolean iscopy;

	const char *event   = env->GetStringUTFChars(aEvent,   &iscopy);
	const char *message = env->GetStringUTFChars(aMessage, &iscopy);

	// if(event_str && msg_str){
		// ((App*)ofGetAppPtr())->javaMessage(event, message);
	// }
}


//--------------------------------------------------------------
extern "C"{

	void Java_cc_openframeworks_OFAndroid_init(JNIEnv*  env, jobject  obj){
		main();
	}

	void Java_org_mpd_OFActivity_javaMessage(JNIEnv* env, jobject obj, jstring aEvent, jstring aMessage){
		javaMessage(env, obj, aEvent, aMessage);
	}

	void Java_org_mpd_CustomDialog_javaMessage(JNIEnv* env, jobject obj, jstring aEvent, jstring aMessage){
		javaMessage(env, obj, aEvent, aMessage);
	}
}
#endif