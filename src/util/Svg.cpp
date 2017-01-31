#include "Svg.h"
#include "svgtiny.h"
#include "ofTypes.h"


//--------------------------------------------------------------
void Svg::loadData(string aData){

	string dummy = "smth";

	struct svgtiny_diagram * diagram = svgtiny_create();

	svgtiny_code code = svgtiny_parse(diagram, aData.c_str(), aData.size(), dummy.c_str(), 0, 0);

	if (code == svgtiny_OK){

		ofLogVerbose() << "loaded";
	}
}
