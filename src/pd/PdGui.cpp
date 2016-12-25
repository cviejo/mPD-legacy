
#include "ofMain.h"
#include "PdGui.h"
#include "PdGuiTypes.h"
#include "PdGuiMessage.h"
// mPD
#include "m_pd.h"
#include "g_canvas.h"
#include "z_libpd.h"
#include "m_imp.h"
#include "g_all_guis.h"
#include "s_stuff.h"


//--------------------------------------------------------------
extern "C" {

	void canvas_mousedown (t_canvas *x, t_floatarg xpos,  t_floatarg ypos,  t_floatarg which, t_floatarg mod);
	void canvas_motion    (t_canvas *x, t_floatarg xpos,  t_floatarg ypos,  t_floatarg fmod);
	void canvas_mouseup   (t_canvas *x, t_floatarg fxpos, t_floatarg fypos, t_floatarg fwhich);
	void canvas_editmode  (t_canvas *x, t_floatarg state);
	void canvas_doclear   (t_canvas *x);
	void canvas_copy      (t_canvas *x);
	void canvas_paste     (t_canvas *x);
	void canvas_undo_undo (t_canvas *x);
	void canvas_duplicate (t_canvas *x);
	void canvas_cut       (t_canvas *x);
	void canvas_menusave  (t_canvas *x);
	void canvas_menusaveas(t_canvas *x);
	void canvas_gridmode  (t_canvas *x, int aState);
	void canvas_gridsize  (t_canvas *x, int cellWidth, int cellHeight);
}


//--------------------------------------------------------------
void gui_msg_hook(char* msg){ PdGui::instance().guiMessage(msg); }


//----------------------------------------------------------
PdGui& PdGui::instance() {
	static PdGui* singletonInstance = new PdGui();
	return * singletonInstance;
}


//--------------------------------------------------------------------
PdGui::PdGui() : PdBase() { this->clear(); }


//--------------------------------------------------------------------
PdGui::~PdGui() { this->clear(); }


//--------------------------------------------------------------------
bool PdGui::init(const int aOutputChannels, const int aInputChannels, const int aSampleRate, const int aTicksPerBuffer, bool aQueued) {

	_ticks          = aTicksPerBuffer;
	_bufferSize     = aTicksPerBuffer*blockSize();
	_sampleRate     = aSampleRate;
	_inputChannels  = aInputChannels;
	_outputChannels = aOutputChannels;
	_inputBuffer    = new float[aInputChannels * _bufferSize];

	if(!PdBase::init(_inputChannels, _outputChannels, _sampleRate, aQueued)) {
		ofLogError("Pd") << "could not init";
		this->clear();
		return false;
	}

	PdBase::setReceiver(this);

	return true;
}


//--------------------------------------------------------------------
PdCanvas* PdGui::openPatch(const string& aPath) {

	ofLogVerbose("Pd") << "opening patch: " + aPath;

	string fullpath = ofFilePath::getAbsolutePath(aPath);
	string file     = ofFilePath::getFileName(fullpath);
	string folder   = ofFilePath::getEnclosingDirectory(fullpath);

	// see ofxPd
	if(folder.size() > 0 && folder.at(folder.size() - 1) == '/') {
		folder.erase(folder.end() - 1);
	}

	Patch  patch   = PdBase::openPatch(file.c_str(), folder.c_str());
	string patchId = getPatchId(patch);

	if (!patch.isValid()){
		ofLogError("Pd") << "opening patch: " + aPath;
	}
	else {
		pd_getcanvaslist()->gl_mapped = 1;
	}

	canvas_editmode(pd_getcanvaslist(), 1);

	return new PdCanvas("");
}


//--------------------------------------------------------------------
void PdGui::closePatch(PdCanvas& aCanvas) {
	
	ofLogVerbose("Pd") << "closing patch: " + aCanvas.patch.filename();
	
	PdBase::closePatch(aCanvas.patch);
}


//--------------------------------------------------------------------
void PdGui::clear() {

	if(_inputBuffer != NULL) {
		delete[] _inputBuffer;
		_inputBuffer = NULL;
	}

	PdContext::instance().clear();

	_ticks          = 0;
	_bufferSize     = 0;
	_sampleRate     = 0;
	_inputChannels  = 0;
	_outputChannels = 0;
}


//----------------------------------------------------------
void PdGui::startAudio() {

	_computing = true;

	PdBase::computeAudio(true);
}


//----------------------------------------------------------
void PdGui::stopAudio() {

	_computing = false;

	PdBase::computeAudio(false);
}


//----------------------------------------------------------
void PdGui::audioIn(float* input, int bufferSize, int nChannels) {

	try {

		if (_inputBuffer == NULL){ return; }

		if(bufferSize != _bufferSize || nChannels != _inputChannels) {
			this->init(_outputChannels, nChannels, _sampleRate, bufferSize/blockSize(), isQueued());
			PdBase::computeAudio(_computing);
		}

		memcpy(_inputBuffer, input, bufferSize*nChannels*sizeof(float));
	}
	catch (...) {
		ofLogError("Pd") << "could not copy input buffer";
	}
}


//----------------------------------------------------------
void PdGui::audioOut(float* output, int bufferSize, int nChannels) {

	if (_inputBuffer == NULL){ return; }

	if(bufferSize != _bufferSize || nChannels != _inputChannels) {
		this->init(_outputChannels, nChannels, _sampleRate, bufferSize/blockSize(), isQueued());
		PdBase::computeAudio(_computing);
	}

	if(!PdBase::processFloat(_ticks, _inputBuffer, output)) {
		ofLogError("Pd") << "could not process output buffer";
	}
}



//--------------------------------------------------------------
vector<PdCanvas*> PdGui::getCanvases(){
	return _canvases;
}


//--------------------------------------------------------------
PdCanvas* PdGui::getCanvas(string aId){

	for (auto cnv : _canvases){

		if(cnv->id == aId){
			return cnv;
		}
	}

	return NULL;
}


//--------------------------------------------------------------
PdNode* PdGui::getNode(string aCanvasId, string aNodeId){

	PdCanvas* canvas = this->getCanvas(aCanvasId);

	if (canvas == NULL){ return NULL; }

	for (auto node : canvas->nodes){

		if(node->id == aNodeId){
			return node;
		}
	}

	return NULL;
}


//--------------------------------------------------------------
void PdGui::canvasPressed(int x, int y){  canvas_mousedown(pd_getcanvaslist(), x, y, 0, 0); }


//--------------------------------------------------------------
void PdGui::canvasDragged(int x, int y){  canvas_motion(pd_getcanvaslist(), x, y, 0); }


//--------------------------------------------------------------
void PdGui::canvasReleased(int x, int y){ canvas_mouseup(pd_getcanvaslist(), x, y, 0); }


//--------------------------------------------------------------
void PdGui::canvasDelete(){ canvas_doclear(pd_getcanvaslist()); }


//--------------------------------------------------------------
void PdGui::canvasUndo(){ canvas_undo_undo(pd_getcanvaslist()); }


//--------------------------------------------------------------
void PdGui::canvasCopy(){ canvas_copy(pd_getcanvaslist()); }


//--------------------------------------------------------------
void PdGui::canvasPaste(){ canvas_paste(pd_getcanvaslist()); }

// move to some utils class / module
//--------------------------------------------------------------
string PdGui::getPatchId(Patch& patch){

	stringstream strm;
	
	strm << static_cast<const void*>(patch.handle());

	return strm.str();
}


//--------------------------------------------------------------
string PdGui::unquote(string& str){

	int front   = str.front() == '"';
	int back    = str.back()  == '"';

	return str.substr(front, str.length() - front - back);
}


//--------------------------------------------------------------
void PdGui::evaluateBuffer(string& aBuffer){

	t_binbuf* buffer = binbuf_new();

	binbuf_text(buffer, (char*)aBuffer.c_str(), aBuffer.length());
	binbuf_eval(buffer, 0, 0, 0);
	binbuf_free(buffer);
}


//--------------------------------------------------------------
void PdGui::guiMessage(string aMsg){

	// ofLogVerbose("pd") << aMsg;

	PdGuiMessage guiMsg(aMsg);

	if (guiMsg.command == "gui_canvas_new"){

		if (this->getCanvas(guiMsg.canvasId) == NULL) {

			auto mapCommand = guiMsg.canvasId + " map 1";

			_canvases.push_back(new PdCanvas(guiMsg.canvasId));

			this->evaluateBuffer(mapCommand);
		}
	}
	else if (guiMsg.command == "gui_gobj_new"){

		auto canvas = this->getCanvas(guiMsg.canvasId);

		if (canvas != NULL){

			PdNode* node = new PdNode(guiMsg.nodeId);

			node->patchId = guiMsg.canvasId;
			node->x       = ofToInt(guiMsg.args[3]);
			node->y       = ofToInt(guiMsg.args[4]);

			canvas->nodes.push_back(node);
		}
	}
	else if (guiMsg.command == "gui_text_draw_border"){

		auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId);

		if (node != NULL){

			guiMsg.parseColor(2);
			guiMsg.parseRect(4);

			node->x = guiMsg.x;
			node->y = guiMsg.y;

			node->setSize(guiMsg.width, guiMsg.height);
			node->backgroundColor = guiMsg.color;
		}
	}
	else if (guiMsg.command == "gui_gobj_draw_io"){
		// "x102162000",".x102162000.t101252050",".x102162000.t101252050",78,337,85,340,78,326,"o",0,0,0
	}
	else if (guiMsg.command == "gui_canvas_line"){

		auto canvas = this->getCanvas(guiMsg.canvasId);

		if (canvas != NULL){

			PdConnection* conn = new PdConnection();

			conn->id = guiMsg.nodeId;
			conn->x1 = ofToInt(guiMsg.args[7]);
			conn->y1 = ofToInt(guiMsg.args[8]);
			conn->x2 = ofToInt(guiMsg.args[10]);
			conn->y2 = ofToInt(guiMsg.args[11]);

			canvas->connections.push_back(conn);
		}
	}
	else if (guiMsg.command == "gui_canvas_update_line"){

		auto canvas = this->getCanvas(guiMsg.canvasId);

		if (canvas != NULL){

			for (auto conn : canvas->connections){

				if(conn->id == guiMsg.nodeId){
					conn->x1 = ofToInt(guiMsg.args[2]);
					conn->y1 = ofToInt(guiMsg.args[3]);
					conn->x2 = ofToInt(guiMsg.args[4]);
					conn->y2 = ofToInt(guiMsg.args[5]);
				}
			}
		}
	}
	else if (guiMsg.command == "gui_gobj_select"){

		auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId);

		if (node != NULL){
			node->selected = true;
		}
	}
	else if (guiMsg.command == "gui_gobj_deselect"){

		auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId);

		if (node != NULL){
			node->selected = false;
		}
	}
	else if (guiMsg.command == "gui_canvas_displace_withtag"){

		auto canvas = this->getCanvas(guiMsg.canvasId);

		if (canvas != NULL){

			for (auto node : canvas->nodes){
				if (node->selected){
					node->x += ofToInt(guiMsg.args[1]);
					node->y += ofToInt(guiMsg.args[2]);
				}
			}
		}
	}
	else if (guiMsg.command == "gui_canvas_move_selection"){

		auto canvas = this->getCanvas(guiMsg.canvasId);

		if (canvas != NULL){

			guiMsg.parseRect(1);

			canvas->region.x = guiMsg.x;
			canvas->region.y = guiMsg.y;
			canvas->region.setSize(guiMsg.width, guiMsg.height);

			canvas->mode = PdCanvas::MODE_REGION;
		}
	}
	else if (guiMsg.command == "gui_canvas_hide_selection"){

		auto canvas = this->getCanvas(guiMsg.canvasId);

		if (canvas != NULL){
			canvas->mode = PdCanvas::MODE_NONE;
		}
	}
	else if (guiMsg.command == "gui_iemgui_label_new"){
		// "x102162000","x102181e00",17,7,"#000000","some label","Monaco","normal",10
	}
	else {
		ofLogVerbose("pd") << aMsg;
	}
}

