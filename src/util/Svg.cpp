#include "Svg.h"
#include "svgtiny.h"
#include "ofTypes.h"


//--------------------------------------------------------------
void Svg::loadData(string aFormat, string aData){

	string dummy = "smth";

	// string xml = "<svg><path fill=\"#FF00FF\" stroke=\"#00FF00\" stroke-width=\"0.172\" d=\"" + aData + "\"/></svg>";
	string xml = "<svg><path " + aFormat + " d=\"" + aData + "\"/></svg>";
	// string xml = "<svg><path " + aFormat + " stroke-width=\"1\" d=\"" + aData + "\"/></svg>";

	// struct svgtiny_diagram * diagram = svgtiny_create();
	auto diagram = svgtiny_create();

	auto code = svgtiny_parse(diagram, xml.c_str(), xml.size(), dummy.c_str(), 0, 0);

	if (code == svgtiny_OK){

		setupDiagram(diagram);

		svgtiny_free(diagram);

		// ofLogVerbose() << "loaded";
	}
}

void Svg::draw(){
	for(int i = 0; i < (int)paths.size(); i++){
		paths[i].draw();
	}
}


void Svg::setupDiagram(struct svgtiny_diagram * diagram){

	width = diagram->width;
	height = diagram->height;

	paths.clear();

	for(int i = 0; i < (int)diagram->shape_count; i++){
		if(diagram->shape[i].path){
			paths.push_back(ofPath());
			setupShape(&diagram->shape[i],paths.back());
		}else if(diagram->shape[i].text){
			ofLogWarning("Svg") << "setupDiagram(): text: not implemented yet";
		}
	}
}

void Svg::setupShape(struct svgtiny_shape * shape, ofPath & path){

	float* p = shape->path;

	path.setFilled(false);

	if(shape->fill != svgtiny_TRANSPARENT){
		path.setFilled(true);
		path.setFillHexColor(shape->fill);
		path.setPolyWindingMode(OF_POLY_WINDING_NONZERO);
    }

	if(shape->stroke != svgtiny_TRANSPARENT){
		path.setStrokeWidth(shape->stroke_width);
		path.setStrokeHexColor(shape->stroke);
	}

	for(int i = 0; i < (int)shape->path_length;){
		if(p[i] == svgtiny_PATH_MOVE){
			path.moveTo(p[i + 1], p[i + 2]);
			i += 3;
		}
		else if(p[i] == svgtiny_PATH_CLOSE){
			path.close();

			i += 1;
		}
		else if(p[i] == svgtiny_PATH_LINE){
			path.lineTo(p[i + 1], p[i + 2]);
			i += 3;
		}
		else if(p[i] == svgtiny_PATH_BEZIER){
			path.bezierTo(p[i + 1], p[i + 2],
						   p[i + 3], p[i + 4],
						   p[i + 5], p[i + 6]);
			i += 7;
		}
		else{
			ofLogError("Svg") << "setupShape(): SVG parse error";
			i += 1;
		}
	}
}
