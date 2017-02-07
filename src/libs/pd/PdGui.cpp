
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
#include "Svg.h"

//--------------------------------------------------------------
extern "C" {

	void canvas_mousedown (t_canvas *x, t_floatarg xpos,  t_floatarg ypos,  t_floatarg which, t_floatarg mod);
	void canvas_motion    (t_canvas *x, t_floatarg xpos,  t_floatarg ypos,  t_floatarg fmod);
	void canvas_mouseup   (t_canvas *x, t_floatarg fxpos, t_floatarg fypos, t_floatarg fwhich);
	void canvas_editmode  (t_canvas *x, t_floatarg state);
	void canvas_doclear   (t_canvas *x);
	void canvas_copy      (t_canvas *x);
	void canvas_paste     (t_canvas *x);
	// void canvas_undo_undo (t_canvas *x);
	void canvas_duplicate (t_canvas *x);
	void canvas_cut       (t_canvas *x);
	void canvas_menusave  (t_canvas *x);
	void canvas_menusaveas(t_canvas *x);
	void canvas_gridmode  (t_canvas *x, int aState);
	void canvas_gridsize  (t_canvas *x, int cellWidth, int cellHeight);
	// void canvas_updatelines(t_canvas *x);
	// void canvas_updateconnections(t_canvas *x, int lx1, int ly1, int lx2, int ly2, t_int tag);
}


//--------------------------------------------------------------
void gui_msg_hook(char* msg){
	// PdGui::instance().lock();
	PdGui::instance().guiMessage(msg);
	// PdGui::instance().unlock();
}


//----------------------------------------------------------
PdGui& PdGui::instance() {
	static PdGui* singletonInstance = new PdGui();
	return * singletonInstance;
}


//--------------------------------------------------------------------
PdGui::PdGui() : PdBase() {

	this->clear();
}


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

	Patch patch = PdBase::openPatch(file.c_str(), folder.c_str());
	// string patchId = getPatchId(patch);

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
void PdGui::closePatch(PdCanvas& aCanvas) { PdBase::closePatch(aCanvas.patch); }


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
void PdGui::canvasPressed(PdCanvas* canvas, int x, int y){ canvas_mousedown((t_canvas*)pd_getcanvaslist(), x, y, 0, 0); }


//--------------------------------------------------------------
void PdGui::canvasDragged(PdCanvas* canvas, int x, int y){ canvas_motion((t_canvas*)pd_getcanvaslist(), x, y, 0); }


//--------------------------------------------------------------
void PdGui::canvasReleased(PdCanvas* canvas, int x, int y){ canvas_mouseup((t_canvas*)pd_getcanvaslist(), x, y, 0); }


// //--------------------------------------------------------------
// void PdGui::canvasDelete(PdCanvas* canvas){ canvas_doclear(pd_getcanvaslist()); }


                    // // else binbuf_eval(inbinbuf, 0, 0, 0);
// //--------------------------------------------------------------
// // void PdGui::canvasUndo(PdCanvas* canvas){ canvas_undo_undo(pd_getcanvaslist()); }
// void PdGui::canvasUndo(PdCanvas* canvas){
	// auto cmd = (_canvases[0])->id + " undo";
	// this->evaluateBuffer(cmd);
// }


// //--------------------------------------------------------------
// void PdGui::canvasCopy(PdCanvas* canvas){ canvas_copy(pd_getcanvaslist()); }


// //--------------------------------------------------------------
// void PdGui::canvasPaste(PdCanvas* canvas){ canvas_paste(pd_getcanvaslist()); }

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
void PdGui::pdsend(string& aCmd, bool aLock){

	if (aLock){
		this->lock();
	}

	t_binbuf* buffer = binbuf_new();

	binbuf_text(buffer, (char*)aCmd.c_str(), aCmd.length());
	binbuf_eval(buffer, 0, 0, 0);
	binbuf_free(buffer);

	if (aLock){
		this->unlock();
	}
	// this->hh(aCmd.c_str(), aCmd.length());
}


//--------------------------------------------------------------
void PdGui::displaceObject(PdNode* aNode, ofPoint aOffset){

	aNode->translate(aOffset);

	if (aNode->type == "iemgui"){

		PdIemGui* guiNode = (PdIemGui*)aNode;

		guiNode->slider.translate(aOffset);

		if (guiNode->canvas){
			guiNode->canvas->translate(aOffset);
		}
		if (guiNode->label){
			guiNode->label->translate(aOffset);
		}

		for (auto& rect : guiNode->radioButtons){
			rect.translate(aOffset);
		}
		for (auto& rect : guiNode->radios){
			rect.translate(aOffset);
		}
	}

	for (auto inlet : aNode->inlets){
		inlet->translate(aOffset);
	}
	for (auto outlet : aNode->outlets){
		outlet->translate(aOffset);
	}
}


// //--------------------------------------------------------------
// PdNode* PdGui::getNewNode(PdGuiMessage& aGuiMsg){

	// // PdNode* node = new PdNode(guiMsg.nodeId);

	// // node->patchId = guiMsg.canvasId;

	// // canvas->nodes.push_back(node);

	// // return node;
// }


//--------------------------------------------------------------
void PdGui::guiMessage(string aMsg){

	PdGuiMessage guiMsg(aMsg);

	if (guiMsg.command == "gui_canvas_new"){
		// gui_canvas_new "x2380d10",1138,688,"+0+52",0,"main.pd","/developer/OF/v0.9.7/apps/pd/mPD/bin/data",0,""
		if (!this->getCanvas(guiMsg.canvasId)) {

			auto mapCommand = guiMsg.canvasId + " map 1";

			_canvases.push_back(new PdCanvas(guiMsg.canvasId));

			this->pdsend(mapCommand, false);
		}
	}
	else if (guiMsg.command == "gui_canvas_set_editmode"){
		// gui_canvas_set_editmode "x7f9244127e00",1

		if (auto canvas = this->getCanvas(guiMsg.canvasId)){
			canvas->editMode = ofToBool(guiMsg.args[1]);
		}
	}
	else if (guiMsg.command == "gui_gobj_new"){
		// gui_gobj_new "x24078d0","x23fa810","iemgui",55,316,1
		if (auto canvas = this->getCanvas(guiMsg.canvasId)){

			string  type = this->unquote(guiMsg.args[2]);
			PdNode* node;

			if (type == "iemgui"){
				node = new PdIemGui(guiMsg.nodeId);
				node->borderColor = 0;
			}
			else if (type == "graph"){
				node = new PdCanvas(guiMsg.nodeId);
				node->borderColor = 204;
				ofLogVerbose() << "graphId " << guiMsg.nodeId;
			}
			else {
				node = new PdNode(guiMsg.nodeId);
				node->borderColor = 204;
			}

			node->patchId = guiMsg.canvasId;
			canvas->nodes.push_back(node);

			node->type = type;
			node->setPosition(ofToInt(guiMsg.args[3]), ofToInt(guiMsg.args[4]));
		}
	}
	else if (guiMsg.command == "gui_gobj_erase"){
		// gui_gobj_erase "x7fbe328f2400","x7fbe32a10400"
		if (auto canvas = this->getCanvas(guiMsg.canvasId)){

			for (auto& node : canvas->nodes){
				if (node->id == guiMsg.nodeId){
					  delete node;
					  node = NULL;
				}
			}

			canvas->nodes.erase(
				remove(canvas->nodes.begin(), canvas->nodes.end(), static_cast<PdNode*>(NULL)),
				end(canvas->nodes)
			);
		}
	}
	else if (guiMsg.command == "gui_numbox_new"){
		// gui_numbox_new "x2380d10","x2345550","xfcfcfc",168,248,54,14,1
		if (auto canvas = this->getCanvas(guiMsg.canvasId)){

			PdNode* node = new PdNode(guiMsg.nodeId);
			node->patchId = guiMsg.canvasId;
			canvas->nodes.push_back(node);

			node->borderColor = 0;
			node->type = "numbox";

			node->setPosition(ofToInt(guiMsg.args[3]), ofToInt(guiMsg.args[4]));
			node->setSize    (ofToInt(guiMsg.args[5]), ofToInt(guiMsg.args[6]));
		}
	}
	else if (guiMsg.command == "gui_mycanvas_new"){
		// gui_mycanvas_new "x2fec990","x3022eb0","xdcdcdc",0,499,550,502,3,502
		if (auto node = (PdIemGui*)this->getNode(guiMsg.canvasId, guiMsg.nodeId)){

			node->setSize(ofToInt(guiMsg.args[7]) - node->x,
			              ofToInt(guiMsg.args[8]) - node->y);

			node->backColor = guiMsg.color;

			node->canvas = new PdNode();
			node->canvas->type = "mycanvas";
			node->canvas->set(guiMsg.parseRect(3));
			node->canvas->backColor = guiMsg.parseColor(2);
		}
	}
	else if (guiMsg.command == "gui_bng_new"){
		// gui_bng_new "x2a31760","x2a79320",7.5,7.5,6.5
		if (auto node = (PdIemGui*)this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->iemType = "bng";
		}
	}
	else if (guiMsg.command == "gui_bng_button_color"){
		// gui_bng_button_color "x1f6ba90","x1fb6ff0","xfcfcfc"
		if (auto node = (PdIemGui*)this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->frontColor = guiMsg.parseColor(2);
		}
	}
	else if (guiMsg.command == "gui_toggle_new"){
		// gui_toggle_new "x268b500","x26ae8c0","x000000",1,0,78,105,89,116,78,116,89,105,76,103
		if (auto node = (PdIemGui*)this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->iemType = "toggle";
		}
	}
	else if (guiMsg.command == "gui_toggle_update"){
		// gui_toggle_update "x2b80f80","x2b3d020",0,"x00fc04"
		if (auto node = (PdIemGui*)this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->iemType    = "toggle";
			node->value      = ofToInt(guiMsg.args[2]);
			node->frontColor = guiMsg.parseColor(3);
		}
	}
	else if (guiMsg.command == "gui_slider_new"){
		// gui_slider_new "x268b500","x26b0d50","x000000",190,166,201,166,188,106
		if (auto node = (PdIemGui*)this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->iemType = "slider";
			node->slider.set(guiMsg.parseRect(3));
		}
	}
	else if (guiMsg.command == "gui_slider_update"){
		// gui_slider_update "x228c5f0","x22b0790",51,231,62,231,49,30
		if (auto node = (PdIemGui*)this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->slider.set(guiMsg.parseRect(2));
		}
	}
	else if (guiMsg.command == "gui_iemgui_label_new"){
		// gui_iemgui_label_new "x102162000","x102181e00",17,7,"#000000","some label","Monaco","normal",10
		if (auto node = (PdIemGui*)this->getNode(guiMsg.canvasId, guiMsg.nodeId)){

			node->label = new PdNode();
			node->text  = this->unquote(guiMsg.args[5]);
			node->textPosition.set(ofToInt(guiMsg.args[2]), ofToInt(guiMsg.args[3]));
			node->label->backColor = guiMsg.color;
		}
	}
	else if (guiMsg.command == "gui_graph_label"){
		// gui_graph_label "x24ba490",".x24ba490.t24b93b0",0,10,"array2","DejaVu Sans Mono",5,"normal",0
		if (auto node = (PdIemGui*)this->getNode(guiMsg.canvasId, guiMsg.nodeId)){

			node->label = new PdNode();
			node->text  = this->unquote(guiMsg.args[4]);
			node->textPosition.set(ofToInt(guiMsg.args[2]), ofToInt(guiMsg.args[3]));
			node->label->backColor = guiMsg.color;
		}
	}
	else if (guiMsg.command == "gui_text_new"){
		// gui_text_new "x7fbe1412a600",".x7fbe1412a600.t7fbe13deb990","atom",0,2,10,"0",5
		if (auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->text = this->unquote(guiMsg.args[6]);
			node->textPosition.set(ofToInt(guiMsg.args[4]), ofToInt(guiMsg.args[5]));
		}
	}
	else if (guiMsg.command == "gui_radio_create_buttons"){
		// gui_radio_create_buttons "x24078d0","x23fa810","x000000",58,334,67,343,55,316,1,0
		if (auto node = (PdIemGui*)this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			if(ofToInt(guiMsg.args[10])){
				node->value = ofToInt(guiMsg.args[9]);
				node->frontColor = guiMsg.parseColor(2);
			}
			node->radioButtons.push_back(guiMsg.parseRect(3));
		}
	}
	else if (guiMsg.command == "gui_radio_update"){
		// gui_radio_update "x13c0de0","x140ec00","xf8fc00",6,4
		if (auto node = (PdIemGui*)this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->value = ofToInt(guiMsg.args[4]);
			node->frontColor = guiMsg.parseColor(2);
		}
	}
	else if (guiMsg.command == "gui_radio_new"){
		// gui_radio_new "x24078d0","x23fa810",55,346,70,346,2,55,316
		if (auto node = (PdIemGui*)this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->iemType = "radio";
			node->radios.push_back(guiMsg.parseRect(2));
		}
	}
	else if (guiMsg.command == "gui_text_set"){
		// gui_text_set "x2380d10",".x2380d10.t234e170","symbol"
		if (auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->text = this->unquote(guiMsg.args[2]);
		}
	}
	else if (guiMsg.command == "gui_text_draw_border"){
		// gui_text_draw_border "x24078d0","x23fa810","#fcfcfc",0,55,316,70,436
		if (auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->set(guiMsg.parseRect(4));
			node->backColor = guiMsg.parseColor(2);
		}
	}
	else if (guiMsg.command == "gui_message_draw_border" || guiMsg.command == "gui_atom_draw_border"){

		if (auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->setSize(ofToInt(guiMsg.args[2]), ofToInt(guiMsg.args[3]));
		}
	}
	else if (guiMsg.command == "gui_gobj_draw_io"){
		// gui_gobj_draw_io "x7fd63490ce00",".x7fd63490ce00.t7fd633ccaa50",".x7fd63490ce00.t7fd633ccaa50",36,37,43,40,36,16,"o",0,0,0
		if (auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId)){

			PdIo* io = new PdIo();

			io->set(guiMsg.parseRect(3));
			io->signal = guiMsg.args[11] != "0";

			if (guiMsg.args[9] == "\"i\"") {
				node->inlets.push_back(io);
			}
			else {
				node->outlets.push_back(io);
			}
		}
	}
	else if (guiMsg.command == "gui_canvas_line"){
		// gui_canvas_line "x18df640","l18abc90","signal",3,0,3,10,15,30,27,50,27,60
		if (auto canvas = this->getCanvas(guiMsg.canvasId)){

			PdConnection* conn = new PdConnection();

			conn->id = guiMsg.nodeId;
			conn->x  = ofToInt(guiMsg.args[3]);
			conn->y  = ofToInt(guiMsg.args[4]);
			conn->x2 = ofToInt(guiMsg.args[11]);
			conn->y2 = ofToInt(guiMsg.args[12]);

			canvas->connections.push_back(conn);
		}
	}
	else if (guiMsg.command == "gui_canvas_update_line"){

		if (auto canvas = this->getCanvas(guiMsg.canvasId)){

			for (auto conn : canvas->connections){

				if(conn->id == guiMsg.nodeId){
					conn->x  = ofToInt(guiMsg.args[2]);
					conn->y  = ofToInt(guiMsg.args[3]);
					conn->x2 = ofToInt(guiMsg.args[4]);
					conn->y2 = ofToInt(guiMsg.args[5]);
				}
			}
		}
	}
	else if (guiMsg.command == "gui_canvas_delete_line"){
		// gui_canvas_delete_line "x7fcd78914400","newcord"
		if (auto canvas = this->getCanvas(guiMsg.canvasId)){

			for (auto& conn : canvas->connections){
				if (conn->id == guiMsg.nodeId){
					  delete conn;
					  conn = NULL;
				}
			}
			canvas->connections.erase(
					remove(canvas->connections.begin(), canvas->connections.end(), static_cast<PdConnection*>(NULL)),
					end(canvas->connections)
			);
		}
	}
	else if (guiMsg.command == "gui_gobj_select" || guiMsg.command == "gui_gobj_deselect"){

		if (auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->selected = guiMsg.command == "gui_gobj_select";
		}
	}
	else if (guiMsg.command == "gui_gobj_displace" || guiMsg.command == "gui_text_displace"){
		// gui_text_displace "x7f7fa9137200",".x7f7fa9137200.t7f7fa84c4960",-1,2
		if (auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			ofPoint offset(ofToInt(guiMsg.args[2]), ofToInt(guiMsg.args[3]));
			this->displaceObject(node, offset);
		}
	}
	else if (guiMsg.command == "gui_iemgui_move_and_resize"){
		// gui_iemgui_move_and_resize "x7ff236121800","x7ff236124a00",100,40,154,54
		if (auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			ofRectangle rect = guiMsg.parseRect(2);
			this->displaceObject(node, rect.getTopLeft() - node->getTopLeft());
			node->setSize(rect.width, rect.height);
		}
	}
	else if (guiMsg.command == "gui_canvas_displace_withtag"){

		if (auto canvas = this->getCanvas(guiMsg.canvasId)){

			ofPoint offset(ofToInt(guiMsg.args[1]), ofToInt(guiMsg.args[2]));

			for (auto& node : canvas->nodes){

				if (node->selected){
					this->displaceObject(node, offset);
				}
			}
		}
	}
	else if (guiMsg.command == "gui_canvas_move_selection"){

		if (auto canvas = this->getCanvas(guiMsg.canvasId)){
			canvas->region.set(guiMsg.parseRect(1));
			canvas->moveMode = PdCanvas::MODE_REGION;
		}
	}
	else if (guiMsg.command == "gui_canvas_hide_selection"){

		if (auto canvas = this->getCanvas(guiMsg.canvasId)){
			canvas->moveMode = PdCanvas::MODE_NONE;
		}
	}
	else if (guiMsg.command == "gui_grid_active"){
		// gui_grid_active "x28d7da0",1
		if (auto canvas = this->getCanvas(guiMsg.canvasId)){
			canvas->gridMode = ofToBool(guiMsg.args[1]);
		}
	}
	else if (guiMsg.command == "gui_grid_size"){

		if (auto canvas = this->getCanvas(guiMsg.canvasId)){
			// canvas->gridMode = ofToBool(guiMsg.args[1]);
			// canvas->moveMode = PdCanvas::MODE_NONE;
		}
	}
	else if (guiMsg.command == "gui_class_new"){

		string className = this->unquote(guiMsg.args[0]);

		if (!ofContains(_classNames, className)){
			_classNames.push_back(className);
		}
	}
	else if (guiMsg.command == "gui_scalar_new"){
		// gui_scalar_new "x24ba490","scalar24ddd18",0,33.3333,0,0,-70,737,240,0

		PdScalar* node   = (PdScalar*)this->getNode(guiMsg.canvasId, guiMsg.nodeId);
		PdCanvas* canvas = (PdCanvas*)this->getCanvas(guiMsg.canvasId);

		if (canvas == NULL){ return; }

		if (node == NULL){
			node = new PdScalar(guiMsg.nodeId);
			canvas->nodes.push_back(node);
		}

		node->setPosition(ofToInt(guiMsg.args[7]), ofToInt(guiMsg.args[8]));
		node->scale.set(ofToInt(guiMsg.args[3]), ofToInt(guiMsg.args[6]));

	}
	else if (guiMsg.command == "gui_scalar_draw_group"){
		// gui_scalar_draw_group "x24ba490","dgroup24921f0.24ddd18","scalar24ddd18gobj"
		
		string scalarId = this->unquote(guiMsg.args[2]);

		ofStringReplace(scalarId, "gobj", "");

		PdCanvas* canvas = (PdCanvas*)this->getCanvas(guiMsg.canvasId);
		PdScalar* scalar = (PdScalar*)this->getNode(guiMsg.canvasId, scalarId);
		PdPath*   path   = (PdPath*)this->getNode(guiMsg.canvasId, guiMsg.nodeId);

		if (path != NULL || scalar == NULL || canvas == NULL){ return; }

		path = new PdPath(guiMsg.nodeId);

		canvas->nodes.push_back(path);
		scalar->paths.push_back(path);
	}
	else if (guiMsg.command == "gui_plot_vis"){

		// TODO: do proper parsing here

		auto pathId = ofSplitString(ofSplitString(aMsg, "[")[3], "\",")[0];

		ofStringReplace(pathId, "\"", "");

		if (auto node = (PdPath*)this->getNode(guiMsg.canvasId, pathId)){

			auto canvas      = (PdCanvas*)this->getCanvas(guiMsg.canvasId);
			auto data        = ofSplitString(ofSplitString(aMsg, "[")[1], "]")[0];
			auto formatParts = ofSplitString(ofSplitString(ofSplitString(aMsg, "[")[2], "]")[0], ",");

			string format = "";

			for (int i = 0; i < formatParts.size() - 1; i+=2){
				if (formatParts[i] != "\"stroke-width\""){
					format += this->unquote(formatParts[i]) + "=" + formatParts[i+1];
				}
				format += " ";
			}

			format += "stroke-width=\"1\"";

			ofStringReplace(data, "\",", "");
			ofStringReplace(data, ",\"", "");
			ofStringReplace(data, "\"",  "");

			node->svg.loadData(format, data);
		}
	}
	else {

		// ofLogVerbose() << aMsg;
		// ofLogVerbose() << guiMsg.command;
		// ofLogVerbose() << "todo:";
	}

	// ofLogVerbose() << aMsg;
}

/*
        gui_vmess("gui_scalar_new", "xsiffffiii",
            glist_getcanvas(owner), 
            tagbuf,
            glist_isselected(owner, &x->sc_gobj),
            xscale, 0.0, 0.0, yscale,
            (int)glist_xtopixels(owner, basex),
            (int)glist_ytopixels(owner, basey),
            glist_istoplevel(owner));
 
[verbose] gui_gobj_new "x24ba490",".x24ba490.t24b93b0","graph",737,170,1
[verbose] gui_text_draw_border "x24ba490",".x24ba490.t24b93b0","none",0,737,170,937,310
[verbose] gui_graph_label "x24ba490",".x24ba490.t24b93b0",0,10,"array2","DejaVu Sans Mono",5,"normal",0
[verbose] gui_scalar_new "x24ba490","scalar24ddd18",0,33.3333,0,0,-70,737,240,0
[verbose] gui_scalar_draw_group "x24ba490","dgroup24921f0.24ddd18","scalar24ddd18gobj"
[verbose] gui_plot_vis "x24ba490",0,6,["M",0,-0.342859,"H",1,"V",-0.37143,"H",0,"z","M",1,0.185715,"H",2,"V",0.157144,"H",1,"z","M",2,0.285716,"H",3,"V",0.257145,"H",2,"z","M",3,0.342859,"H",4,"V",0.314288,"H",3,"z","M",4,0.671433,"H",5,"V",0.642862,"H",4,"z","M",5,-0.800005,"H",6,"V",-0.828576,"H",5,"z"],["fill","black","stroke","black","stroke-width",0],["dgroup24921f0.24ddd18","dgroup24921f0.24ddd18"]
[verbose] gui_find_lowest_and_arrange "x24ba490",".x24ba490.t24b97c0",".x24ba490.x24dc190.template24ddd18"

*/
