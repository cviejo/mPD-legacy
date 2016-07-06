#include "ofMain.h"
#include "../gui/GUIElement.h"

#ifdef WIN32

namespace pd {

//--------------------------------------------------------------
enum CanvasMode {
	CANVAS_MODE_NONE = 0,
	CANVAS_MODE_MOVE,
	CANVAS_MODE_CONNECT,
	CANVAS_MODE_REGION
};


//--------------------------------------------------------------
class PdNode : public ofRectangle
{
	public:

		bool         selected  = false;
		bool         valid     = true;
		bool         control   = false;
		string       className = "";
		int          value     = 0;
		string       content;
		string       label;
		ofPoint      labelLoc;
		vector<bool> inlets;
		vector<bool> outlets;

		PdNode(string aContent) : content(aContent) {}
};


//--------------------------------------------------------------
class PdReceiver { };


//--------------------------------------------------------------
class PdMock {

	public:

		ofPoint        pressLoc;
		ofPoint        mouseLoc;

		bool           init(const int outChannels, const int inChannels, const int rate, const int ticks=32, bool queued=false) { return true; }
		void           start() {}
		void           openPatch(string aPatch) {}
		void           addReceiver(PdReceiver& receiver) {}
		void           audioIn (float * output, int bufferSize, int nChannels) { }
		void           audioOut(float * output, int bufferSize, int nChannels) { }

		int            getBlockSize() { return 64; }

		vector<string> getObjectNames(){ vector<string> result; return result; }
		vector<PdNode> getSelection()  { vector<PdNode> result; return result; }
		vector<PdNode> getNodes()      { 
			vector<PdNode> result; 
			PdNode node("");

			node.x = 30;
			node.y = 30;
			node.width = 40;
			node.height = 40;
			result.push_back(node);
			return result;
		}

		void           canvasPressed(ofPoint point) { pressLoc.set(point); }
		void           canvasDragged(ofPoint point) { mouseLoc.set(point); }
		void           canvasReleased(ofPoint point){ return; }
		void           canvasPressed(int x, int y)  { return; }
		void           canvasDragged(int x, int y)  { return; }
		void           canvasReleased(int x, int y) { return; }
		CanvasMode     getCanvasMode() { return CANVAS_MODE_NONE; }
		bool           getNodePressed(){ return false; }
		bool           getCanvasSelecting(){ return true; }
		bool           getCanvasEditMode() { return false; }
		void           setCanvasEditMode(bool aState){ return; }
		void           setCanvasGridMode(bool aState){ return; }
		void           setCanvasGridSize(int aCellWidth, int aCellHeight){ return; }
};

}

#endif