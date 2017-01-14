#pragma once

#include "ofMain.h"
#include "GuiElement.h"
#include "PdGui.h"
// #include "../Globals.h"
// #include "../util/Font.h"
// #include "../util/AppEvent.h"
// #include "./canvas-renderers/CanvasRenderer.h"


//--------------------------------------------------------------
class Canvas : public GuiElement {

	public:

		// bool scaling = false;

		Canvas();

		virtual void draw();
		virtual void onPressed    (int aX, int aY, int aId);
		virtual void onDragged    (int aX, int aY, int aId);
		virtual void onReleased   (int aX, int aY, int aId);
		virtual void onDoubleClick(int aX, int aY);
		virtual void onAppEvent(AppEvent& aAppEvent);
		virtual void onPressCancel();
		void         set(PdCanvas* canvas);

		// void print(const string& message); // pd receiver callbacks
		// void setFocus(int x, int y);

	private:

		PdCanvas*      _current = NULL;
		ofFbo          _grid;
		ofTrueTypeFont _font;
		ofPoint        _previousMouse;
		PdIo*          _connectionStart;
		// float          _scale;
		// ofRectangle    _viewPort; // in PD coordinates

		// CanvasRenderer* _renderer;
		// ofPoint         _mouseLoc;   // stored in PD coordinates
		// ofPoint         _pressLoc;   // stored in PD coordinates
		// ofPoint         _draggedLoc; // stored in mPD coordinates (only relevant to mPD)
		// ofPoint         _offsetLoc;  // stored in mPD coordinates (only relevant to mPD)
		// Font            _consoleFont;
		// vector<string>  _logs;

		void    initGrid();
		// void    clipOffset();
		void    drawGrid();
		// void    drawCanvas(PdNode* aCanvas);
		void    drawNodes();
		void    drawConnections();
		void    drawRegion();
		void    drawNodeBackground(PdNode* aNode);
		void    drawNodeText(PdNode* aNode);
		void    drawNodeIo(PdIo* aIo);
		// void    drawConsole();
		// void    drawConnecting();
		// void    drawControl(PdNode& aNode);
		// void    drawControlBackground(PdNode& aNode);
		// void    drawNodeText(PdNode& aNode);
		// void    drawNodeLabel(PdNode& aNode);
		// void    drawIO(PdNode& aNode, bool aInlet);
		// ofPoint transformLoc(float aX, float aY, CoordinateTransformation aTransform);

		// void    drawMessage(PdNode& aNode);
		// void    drawAtom(PdNode& aNode);
		void    sendMouseEvent(string aEventType, ofPoint aLoc);
		PdNode* getNodeAtPosition(int aX, int aY);
		ofPoint transformToPdCoordinates(float aX, float aY);
};

