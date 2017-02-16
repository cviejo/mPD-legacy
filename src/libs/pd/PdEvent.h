#pragma once


#include "ofMain.h"
#include "PdGuiTypes.h"


//--------------------------------------------------------------
class PdEvent : public ofEventArgs {

	public:

		enum EventType {
			TYPE_CANVAS_OPEN = 0,
			TYPE_CANVAS_EDIT
		};

		static ofEvent<PdEvent> events;

		EventType type;
		PdCanvas* canvas;
		// string    message;
		// float     value;
		// int       x;
		// int       y;

		PdEvent(EventType aType, PdCanvas* aCanvas = NULL) : type(aType), canvas(aCanvas){}
};

