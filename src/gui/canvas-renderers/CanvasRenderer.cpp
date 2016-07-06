#include "CanvasRenderer.h"


//--------------------------------------------------------------
void CanvasRenderer::drawCanvas(PdNode& aCanvas){

	ofFill();

	float lineHeight = font.lineHeight / 50.0f * Globals::Theme.node.font.height;

	for (auto node : aCanvas.nodes){

		if (node.comment){
			ofSetColor(Globals::Theme.node.color.front);
			this->drawNodeText(node.content, node.x + 1, node.bottom, Globals::Theme.node.font.height);
			continue;
		}

		if (node.selected){
			ofSetColor(240, 0, 0);
			ofDrawRectangle(node.x - 3, node.y - 3, node.width + 6, node.height + 6);
		}

		if (!node.control){
			drawNodeBackground(node);
			// ofSetColor(Globals::Theme.node.color.front);
			ofSetColor(0);
			this->drawNodeText(node.content,
			                   node.x + 1,
			                   node.y + Globals::Theme.node.io.height + Globals::Theme.node.font.height,
			                   // node.bottom - Globals::Theme.node.io.height - 1,
			                   Globals::Theme.node.font.height);
			this->drawIO(node, true);
			this->drawIO(node, false);
		}
	}
}


//--------------------------------------------------------------
void CanvasRenderer::drawNodeText(string aText, int aX, int aY, int aSize){

	float fontScale = (float)aSize / 50.0f;

	ofPushMatrix();
	ofTranslate(aX, aY);
	ofScale(fontScale, fontScale);
	font.draw(aText, 0, 0);
	ofPopMatrix();
}


//--------------------------------------------------------------
void CanvasRenderer::drawNodeBackground(PdNode& aNode){

	ofFill();

	if (aNode.className == "gatom"){

		// ofSetPolyMode(OF_POLY_WINDING_NONZERO);
		ofSetColor(Globals::Theme.node.color.border);
		ofBeginShape();
			ofVertex(aNode.x, aNode.y);
			ofVertex(aNode.right - 4, aNode.y);
			ofVertex(aNode.right, aNode.y + 4);
			ofVertex(aNode.right, aNode.bottom);
			ofVertex(aNode.x, aNode.bottom);
		ofEndShape();

		ofSetColor(255);
		ofBeginShape();
			ofVertex(aNode.x + 1, aNode.y + 1);
			ofVertex(aNode.right - 4, aNode.y + 1);
			ofVertex(aNode.right - 1, aNode.y + 4);
			ofVertex(aNode.right - 1, aNode.bottom - 1);
			ofVertex(aNode.x + 1, aNode.bottom - 1);
		ofEndShape();
	}
	else if (aNode.className == "message"){

		ofSetColor(Globals::Theme.node.color.border);
		ofBeginShape();
			ofVertex(aNode.x, aNode.y);
			ofVertex(aNode.right + 4, aNode.y);
			ofVertex(aNode.right, aNode.y + 4);
			ofVertex(aNode.right, aNode.bottom - 4);
			ofVertex(aNode.right + 4, aNode.bottom);
			ofVertex(aNode.x, aNode.bottom);
		ofEndShape();

		ofSetColor(255);
		ofBeginShape();
			ofVertex(aNode.x + 1, aNode.y + 1);
			ofVertex(aNode.right + 2, aNode.y + 1);
			ofVertex(aNode.right - 1, aNode.y + 4);
			ofVertex(aNode.right - 1, aNode.bottom - 4);
			ofVertex(aNode.right + 2, aNode.bottom - 1);
			ofVertex(aNode.x + 1, aNode.bottom - 1);
		ofEndShape();
	}
	else {

		ofSetColor(aNode.valid ? Globals::Theme.node.color.border : 
		                         Globals::Theme.node.color.invalid);
		ofDrawRectangle(aNode.x, aNode.y, aNode.width, aNode.height);

		ofSetColor(Globals::Theme.node.color.background);
		ofDrawRectangle(aNode.x + 1, aNode.y + 1, aNode.width - 2, aNode.height - 2);
	}
}


//--------------------------------------------------------------
void CanvasRenderer::drawIO(PdNode& aNode, bool aInlet){

	int          ioWidth  = 7;
	int          ioHeight = aNode.control ? 1 : 3;//Globals::Theme.node.io.height;
	int          ioY      = aInlet ? aNode.y : aNode.bottom - ioHeight;
	vector<bool> io       = aInlet ? aNode.inlets : aNode.outlets;
	int          count    = io.size();
	
	if (count){

		int interval = count > 1 ? (aNode.width - ioWidth) / (count-1) : 0;

		for(int i = 0; i < count; i++){

			int ioX = aNode.x + interval * i;

			if(count > 1 && i == count - 1){
				ioX = aNode.right - ioWidth;
			}

			if (io[i]){

				ofSetColor(Globals::Theme.connection.color.signal);
				ofDrawRectangle(ioX, ioY, ioWidth, ioHeight);
			}
			else {

				// ofSetColor(Globals::Theme.connection.color.control);
				ofSetColor(0);
				ofDrawRectangle(ioX, ioY, ioWidth, ioHeight);

				ofSetColor(Globals::Theme.node.color.background);
				ofDrawRectangle(ioX + 1, ioY + 1, ioWidth - 2, ioHeight - 2);
			}
		}
	}
}
