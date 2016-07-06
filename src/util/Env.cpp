#include "./Env.h"
#ifdef TARGET_ANDROID
#include "ofxAndroid.h"
#endif //TARGET_ANDROID


//--------------------------------------------------------------
float Env::getDPI(){

#ifdef TARGET_ANDROID
	vector<string> args;
	args.push_back("getDPI");
	return ofToFloat(Env::message(args));
#else
	return 200;
#endif
}


//--------------------------------------------------------------
string Env::getStoragePath(){

	vector<string> args;
	args.push_back("getStoragePath");
	return Env::message(args);
}


//--------------------------------------------------------------
string Env::message(vector<string> aArgs){

	string result = "";

#ifdef TARGET_ANDROID

	JNIEnv* env      = ofGetJNIEnv();
	jclass  activity = env->FindClass("org/mpd/OFActivity");
	// jclass  activity = ofGetJavaOFAndroid();

	jobjectArray array = env->NewObjectArray(aArgs.size(), 
	                                         env->FindClass("java/lang/String"),
	                                         env->NewStringUTF(""));

	vector<jstring> items;

	for(int i = 0; i < aArgs.size(); i++){
		items.push_back(env->NewStringUTF(aArgs[i].c_str()));
		env->SetObjectArrayElement(array, i, items.back());
	}

	jmethodID methodId = env->GetStaticMethodID(activity, "onNativeMessage", "([Ljava/lang/String;)Ljava/lang/String;");
	jstring   jresult  = (jstring)env->CallStaticObjectMethod(activity, methodId, array);

	for (auto &item : items){
		env->DeleteLocalRef((jobject)item);
	}

	jboolean copy;
	result = env->GetStringUTFChars(jresult, &copy);
#endif

	ofLog(OF_LOG_VERBOSE, "[jni] res:" + result);

	return result;
}


	// jString tmp;
	// char *stringA = "Test1";
	// char *stringB = "Test2";
	// jclass clsString; 
	// jint size = 2;

	// clsString = (*env)->FindClass(env, "java/lang/String");
	// stringArray = (*env)->NewObjectArray(env, size, clsString, 0);

	// tmp = (*env)->NewStringUTF(env, stringB);
	// (*env)->SetObjectArrayElement(env, stringArray, 0, tmp);

	// tmp = (*env)->NewStringUTF(env, stringA);
	// (*env)->SetObjectArrayElement(env, stringArray, 1, tmp);

	// obj = (*env)->NewObject(env, jClass, MID_init, stringArray);
	// string aMessage = "sup";

	// JNIEnv*   env      = ofGetJNIEnv();
	// jclass    activity = env->FindClass("org/mpd/OFActivity");
	// jmethodID method   = env->GetStaticMethodID(activity, "ddd", "(Ljava/lang/String;)Ljava/lang/String;");
	// jstring   message  = env->NewStringUTF(aMessage.c_str());
//	
	// jstring jresult = (jstring)env->CallStaticObjectMethod(activity, method, message);
//	env->DeleteLocalRef((jobject)arg1);
//	env->DeleteLocalRef((jobject)jStr);
//

