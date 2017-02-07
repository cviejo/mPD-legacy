#include "ofMain.h"
#include "App.h"
#ifdef TARGET_ANDROID
#include <jni.h>
#endif


//--------------------------------------------------------------
int main( ){

	ofSetupOpenGL(1024, 768, OF_WINDOW);
	// ofSetupOpenGL(200, 200, OF_WINDOW);
	ofRunApp(new App());
}


#ifdef TARGET_ANDROID
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

// [verbose] gui_gobj_new "x1299290",".x1299290.t12fac10","graph",415,236,1
// [verbose] gui_text_draw_border "x1299290",".x1299290.t12fac10","none",0,415,236,615,376
// [verbose] gui_graph_label "x1299290",".x1299290.t12fac10",0,10,"array2","DejaVu Sans Mono",5,"normal",0
// [verbose] gui_scalar_new "x1299290","scalar12ae3a8",0,66.6667,0,0,-70,415,306,0
// [verbose] gui_scalar_draw_group "x1299290","dgroup1271020.12ae3a8","scalar12ae3a8gobj"
// [verbose] gui_plot_vis "x1299290",0,6,["M",0,-0.342859,"H",1,"V",-0.37143,"H",0,"z","M",1,0.185715,"H",2,"V",0.157144,"H",1,"z","M",2,0.285716,"H",3,"V",0.257145,"H",2,"z"],["fill","black","stroke","black","stroke-width",0],["dgroup1271020.12ae3a8","dgroup1271020.12ae3a8"]
// [verbose] gui_find_lowest_and_arrange "x1299290",".x1299290.t12fb170",".x1299290.x12ad0b0.template12ae3a8"
