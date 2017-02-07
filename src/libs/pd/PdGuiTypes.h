#pragma once



#include "ofMain.h"
#include "PdTypes.hpp"
#include "Svg.h"


using namespace pd;


//--------------------------------------------------------------
class PdIo : public ofRectangle
{
	public:

		string id;
		bool   signal;
};


//--------------------------------------------------------------
class PdConnection : public PdIo
{
	public:

		int x2;
		int y2;
};


//--------------------------------------------------------------
class PdNode : public ofRectangle
{
	public:

		// enum NodeType {
			// TYPE_TEXT = 0,
			// TYPE_CONTROL,
			// TYPE_GRAPH,
			// TYPE_COMMENT,
			// TYPE_INVALID,
		// };

		int           backColor   = 255;
		int           frontColor  = 0;
		int           borderColor = 0;
		bool          selected    = false;
		string        id;
		string        type;
		string        patchId;
		vector<PdIo*> inlets;
		vector<PdIo*> outlets;
		string        text;
		ofPoint       textPosition;

		PdNode(string aId = ""){
			this->id = aId;
		}

		~PdNode(){
			for (auto it = inlets.begin(); it != inlets.end(); ++it){
				 delete *it;
			}
			for (auto it = outlets.begin(); it != outlets.end(); ++it){
				 delete *it;
			}
		}
};


//--------------------------------------------------------------
class PdIemGui : public PdNode
{
	public:

		string              iemType = "";
		int                 value   = 0;
		PdNode*             label   = NULL;
		PdNode*             canvas  = NULL;
		ofRectangle         slider;
		vector<ofRectangle> radios;
		vector<ofRectangle> radioButtons;
	
		PdIemGui(string aId = "") : PdNode(aId) { }

		~PdIemGui(){
			if (label  != NULL){ delete label; }
			if (canvas != NULL){ delete canvas; }
		}
};


//--------------------------------------------------------------
class PdCanvas : public PdNode {

	public:

		enum CanvasMouseMode {
			MODE_NONE = 0,
			MODE_DRAG,
			MODE_CONNECT,
			MODE_REGION
		};

		float                 scale    = 1.0f;
		bool                  editMode = false;
		bool                  gridMode = false;
		CanvasMouseMode       moveMode;
		Patch                 patch;
		vector<PdNode*>       nodes;
		vector<PdConnection*> connections;
		ofRectangle           region;
		ofRectangle           viewPort;

		PdCanvas(string aId) : PdNode(aId) {
			this->viewPort.setSize(ofGetWidth(), ofGetHeight());
		}
};


//--------------------------------------------------------------
class PdPath : public PdNode {

	public:

		string data;
		Svg    svg;

		PdPath(string aId) : PdNode(aId) {
			this->type = "path";
		}
};


//--------------------------------------------------------------
class PdScalar : public PdNode {

	public:

		vector<PdPath*> paths;
		ofPoint         scale;

		PdScalar(string aId) : PdNode(aId) {
			this->type = "scalar";
		}
};
// MODE_DRAG, kinda wrong here

		// enum NodeType {
			// TYPE_TEXT = 0,
			// TYPE_CONTROL,
			// TYPE_GRAPH,
			// TYPE_COMMENT,
			// TYPE_INVALID,
		// };


		// bool           selected         = false;
		// // bool           control          = false;
		// // bool           comment          = false;
		// // bool           graph            = false;
		// // bool           valid            = true;
		// string         content          = "";
		// NodeType       type             = TYPE_TEXT;
		// string         className        = "";
		// int            value            = 0;
		// int            valueMax         = 0;
		// int            bottom           = 0;
		// int            right            = 0;
		// int            backgroundWidth  = 0; // t_my_canvas
		// int            backgroundHeight = 0; // t_my_canvas
		// int            labelColor       = 0;
		// int            frontColor       = 0;
		// int            backgroundColor  = 0;
		// string         label            = "empty";
		// ofPoint        labelLoc;
		// int            labelFontSize;
		// t_gobj*        object;
		// t_canvas*      parent;

		// vector<PdNode>       nodes; // for canvases
		// vector<PdConnection> connections;

		// PdNode (){}

		// PdNode (t_gobj* aObject, t_canvas* aParent = NULL){

			// this->className = aObject->g_pd->c_name->s_name;
			// this->object    = aObject;
			// this->parent    = aParent;
		// }
