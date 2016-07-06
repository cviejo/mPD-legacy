#pragma once

#include "../../Globals.h"
#include "../../util/Font.h"


class CanvasRenderer {

	public:

		Font font;

		virtual void drawCanvas(PdNode& aCanvas);
		virtual void drawNode(PdNode& aNode){}
		virtual void drawNodeBackground(PdNode& aNode);
		virtual void drawIO(PdNode& aNode, bool aInlet);

		void drawNodeText(string aText, int aX, int aY, int aSize);

	private:

		// void drawNodeBackground(PdNode& aNode){

		// 	ofFill();

		// 	ofSetColor(aNode.valid ? Globals::Theme.node.color.border : 
		// 	                         Globals::Theme.node.color.invalid);
		// 	ofDrawRectangle(aNode.x, aNode.y, aNode.width, aNode.height);

		// 	ofSetColor(Globals::Theme.node.color.background);
		// 	ofDrawRectangle(aNode.x,
		// 	                aNode.y + Globals::Theme.node.io.height,
		// 	                aNode.width,
		// 	                aNode.height - Globals::Theme.node.io.height * 2);
		// }
};
