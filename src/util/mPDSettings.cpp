#include "./mPDSettings.h"


//--------------------------------------------------------------
void mPDSettings::load(string aPath){

	this->loadFile(aPath);

	this->gridActive = this->getBool("settings:grid-active");
	this->tabLock    = this->getFloat("settings:tab-lock");
	this->scale      = this->getDouble("settings:scale");
}


//--------------------------------------------------------------
void mPDSettings::save(){

	// this->gridActive = this->getBool("settings:grid-active");
	// this->tabLock    = this->getFloat("settings:tab-lock");
	// this->scale      = this->getFloat("settings:scale");

	this->xml.setValue("settings:scale", this->scale);
	this->saveFile();
}
