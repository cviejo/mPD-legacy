#pragma once


#include "./XmlWrapper.h"


//--------------------------------------------------------------
class mPDSettings : public XmlWrapper {

	public:

		bool   gridActive = true;
		bool   tabLock    = false;
		float  scale      = 1.0f;

		void load(string aPath);
		void save();
};
