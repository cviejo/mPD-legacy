#pragma once


#include "ofMain.h"
#include "PdTypes.hpp"


using namespace pd;


//--------------------------------------------------------------
class PdConnection
{
	public:

		string id;
		bool   signal;
		int    x1, y1, x2, y2;
};


//--------------------------------------------------------------
class PdIo : public ofRectangle
{
	public:

		string id;
		bool   signal;
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

		int          backgroundColor = 255;
		bool         selected        = false;
		string       id;
		string       type;
		string       patchId;
		vector<PdIo> inlets;
		vector<PdIo> outlets;
		string       text;
		ofPoint      textPosition;

		PdNode(string aId){
			this->id = aId;
		}
};


//--------------------------------------------------------------
class PdIemGui : public PdNode
{
	public:

		int    value = 0;
		PdNode label;
};


//--------------------------------------------------------------
class PdCanvas : public PdNode {

	public:

		enum CanvasMode {
			MODE_NONE = 0,
			MODE_MOVE,
			MODE_CONNECT,
			MODE_REGION
		};

		CanvasMode            mode;
		Patch                 patch;
		vector<PdNode*>       nodes;
		vector<PdConnection*> connections;
		ofRectangle           region;

		PdCanvas(string aId) : PdNode(aId) {}
};


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
