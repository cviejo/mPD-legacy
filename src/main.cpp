#include "ofMain.h"
#include "App.h"
#ifdef TARGET_ANDROID
#include <jni.h>
#endif


//--------------------------------------------------------------
int main( ){

#if defined(TARGET_OF_IOS)
	ofiOSWindowSettings settings;

	settings.enableRetina = true; // enables retina resolution if the device supports it.
	settings.enableDepth = false; // enables depth buffer for 3d drawing.
	settings.enableAntiAliasing = true; // enables anti-aliasing which smooths out graphics on the screen.
	settings.numOfAntiAliasingSamples = 4; // number of samples used for anti-aliasing.
	settings.enableHardwareOrientation = false; // enables native view orientation.
	settings.enableHardwareOrientationAnimation = false; // enables native orientation changes to be animated.
	settings.glesVersion = OFXIOS_RENDERER_ES1; // type of renderer to use, ES1, ES2, etc.

	ofCreateWindow(settings);
#else
	ofSetupOpenGL(1024, 768, OF_WINDOW);
#endif

	return ofRunApp(new App());
}


#if defined(TARGET_ANDROID)
extern "C"{

//--------------------------------------------------------------
void Java_cc_openframeworks_OFAndroid_init( JNIEnv*  env, jobject  thiz ){

	main();
}


//--------------------------------------------------------------
void Java_org_mpd_OFActivity_javaMessage(JNIEnv* env, jobject obj, jstring aEvent, jstring aMessage){

	// javaMessage(env, obj, aEvent, aMessage);
}


}
#endif

