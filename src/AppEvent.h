#pragma once


#include "ofMain.h"


//--------------------------------------------------------------
class AppEvent : public ofEventArgs {

	public:

		enum EventType {
			TYPE_KEY_PRESSED = 0,
			TYPE_BUTTON_PRESSED,
			TYPE_SCROLLER_LETTER_CHANGED,
			TYPE_CREATE_OBJECT,
			TYPE_TAB_STATE_CHANGED,
			TYPE_PD_PRINT,
			TYPE_SCALE_BEGIN,
			TYPE_SCALE,
			TYPE_SCALE_END
		};

		static ofEvent<AppEvent> events;

		EventType type;
		string    message;
		float     value;
		int       x;
		int       y;

		AppEvent(EventType aType, string aMsg = "")            : type(aType), message(aMsg){}
		AppEvent(EventType aType, float aFloat)                : type(aType), value (aFloat){}
		AppEvent(EventType aType, string aMsg, float aFloat)   : type(aType), message(aMsg), value (aFloat){}
		AppEvent(EventType aType, string aMsg, int aX, int aY) : type(aType), message(aMsg), x(aX), y(aY){}
};

