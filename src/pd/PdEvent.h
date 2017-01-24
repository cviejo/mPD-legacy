#pragma once


#include "ofMain.h"


//--------------------------------------------------------------
class PdEvent : public ofEventArgs {

	public:

		enum EventType {
			TYPE_CANVAS_OPEN = 0,
			TYPE_CANVAS_EDIT
		};

		static ofEvent<PdEvent> events;

		EventType type;
		// string    message;
		// float     value;
		// int       x;
		// int       y;

		// PdEvent(EventType aType, string aMsg = "")            : type(aType), message(aMsg){}
		// PdEvent(EventType aType, float aFloat)                : type(aType), value (aFloat){}
		// PdEvent(EventType aType, string aMsg, float aFloat)   : type(aType), message(aMsg), value (aFloat){}
		// PdEvent(EventType aType, string aMsg, int aX, int aY) : type(aType), message(aMsg), x(aX), y(aY){}
};

