
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
	// void canvas_undo_undo (t_canvas *x);
	void canvas_duplicate (t_canvas *x);
	void canvas_cut       (t_canvas *x);
	void canvas_menusave  (t_canvas *x);
	void canvas_menusaveas(t_canvas *x);
	void canvas_gridmode  (t_canvas *x, int aState);
	void canvas_gridsize  (t_canvas *x, int cellWidth, int cellHeight);
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
void PdGui::pdsend(string& aCmd){

	t_binbuf* buffer = binbuf_new();

	binbuf_text(buffer, (char*)aCmd.c_str(), aCmd.length());
	binbuf_eval(buffer, 0, 0, 0);
	binbuf_free(buffer);
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

	ofLogVerbose("pd") << aMsg;

	PdGuiMessage guiMsg(aMsg);

	if (guiMsg.command == "gui_canvas_new"){

		if (!this->getCanvas(guiMsg.canvasId)) {

			auto mapCommand = guiMsg.canvasId + " map 1";

			_canvases.push_back(new PdCanvas(guiMsg.canvasId));

			this->pdsend(mapCommand);
		}
	}
	else if (guiMsg.command == "gui_canvas_set_editmode"){
		// gui_canvas_set_editmode "x7f9244127e00",1

		if (auto canvas = this->getCanvas(guiMsg.canvasId)){
			canvas->editMode = ofToBool(guiMsg.args[1]);
		}
	}
	else if (guiMsg.command == "gui_gobj_new"){

		if (auto canvas = this->getCanvas(guiMsg.canvasId)){

			string  type = this->unquote(guiMsg.args[2]);
			PdNode* node;

			if (type == "iemgui"){
				node = new PdIemGui(guiMsg.nodeId);
				node->borderColor = 0;
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

			guiMsg.parseColor(2);
			guiMsg.parseRect(3);

			node->setSize(ofToInt(guiMsg.args[7]) - node->x,
			              ofToInt(guiMsg.args[8]) - node->y);

			node->backgroundColor = guiMsg.color;

			node->canvas = new PdNode();
			node->canvas->type = "mycanvas";
			node->canvas->set(guiMsg);
			node->canvas->backgroundColor = guiMsg.color;
		}
	}
	else if (guiMsg.command == "gui_slider_new"){
		// gui_slider_new "x268b500","x26b0d50","x000000",190,166,201,166,188,106
		if (auto node = (PdIemGui*)this->getNode(guiMsg.canvasId, guiMsg.nodeId)){

			guiMsg.parseRect(3);

			node->iemType = "slider";
			node->slider.set(guiMsg);
		}
	}
	else if (guiMsg.command == "gui_slider_update"){
		// gui_slider_update "x228c5f0","x22b0790",51,231,62,231,49,30
		if (auto node = (PdIemGui*)this->getNode(guiMsg.canvasId, guiMsg.nodeId)){

			guiMsg.parseRect(2);

			node->slider.set(guiMsg);
		}
	}
	else if (guiMsg.command == "gui_iemgui_label_new"){
		// "x102162000","x102181e00",17,7,"#000000","some label","Monaco","normal",10
		if (auto node = (PdIemGui*)this->getNode(guiMsg.canvasId, guiMsg.nodeId)){

			node->label = new PdNode();
			node->text  = this->unquote(guiMsg.args[5]);
			node->textPosition.set(ofToInt(guiMsg.args[2]), ofToInt(guiMsg.args[3]));
			node->label->backgroundColor = guiMsg.color;
		}
	}
	else if (guiMsg.command == "gui_text_new"){
		// gui_text_new "x7fbe1412a600",".x7fbe1412a600.t7fbe13deb990","atom",0,2,10,"0",5
		if (auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->text = this->unquote(guiMsg.args[6]);
			node->textPosition.set(ofToInt(guiMsg.args[4]), ofToInt(guiMsg.args[5]));
		}
	}
	else if (guiMsg.command == "gui_text_set"){

		if (auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->text = this->unquote(guiMsg.args[2]);
		}
	}
	else if (guiMsg.command == "gui_text_draw_border"){

		if (auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId)){

			guiMsg.parseColor(2);
			guiMsg.parseRect(4);

			node->set(guiMsg);
			node->backgroundColor = guiMsg.color;
		}
	}
	else if (guiMsg.command == "gui_message_draw_border" || guiMsg.command == "gui_atom_draw_border"){

		if (auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->setSize(ofToInt(guiMsg.args[2]), ofToInt(guiMsg.args[3]));
		}
	}
	else if (guiMsg.command == "gui_gobj_draw_io"){
		// "x7fd63490ce00",".x7fd63490ce00.t7fd633ccaa50",".x7fd63490ce00.t7fd633ccaa50",36,37,43,40,36,16,"o",0,0,0
		if (auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId)){

			guiMsg.parseRect(3);

			PdIo io;

			io.set(guiMsg);
			io.signal = guiMsg.args[11] != "0";

			if (guiMsg.args[9] == "i") {
				node->inlets.push_back(io);
			}
			else {
				node->outlets.push_back(io);
			}
		}
	}
	else if (guiMsg.command == "gui_canvas_line"){

		if (auto canvas = this->getCanvas(guiMsg.canvasId)){

			PdConnection* conn = new PdConnection();

			conn->id = guiMsg.nodeId;
			conn->x  = ofToInt(guiMsg.args[7]);
			conn->y  = ofToInt(guiMsg.args[8]);
			conn->x2 = ofToInt(guiMsg.args[10]);
			conn->y2 = ofToInt(guiMsg.args[11]);

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
			// this->unlock();
		}
	}
	else if (guiMsg.command == "gui_gobj_select" || guiMsg.command == "gui_gobj_deselect"){

		if (auto node = this->getNode(guiMsg.canvasId, guiMsg.nodeId)){
			node->selected = guiMsg.command == "gui_gobj_select";
		}
	}
	else if (guiMsg.command == "gui_canvas_displace_withtag"){

		if (auto canvas = this->getCanvas(guiMsg.canvasId)){

			ofPoint offset(ofToInt(guiMsg.args[1]), ofToInt(guiMsg.args[2]));

			for (auto& node : canvas->nodes){

				if (node->selected){

					node->translate(offset);

					if (node->type == "iemgui"){

						PdIemGui* guiNode = (PdIemGui*)node;

						guiNode->slider.translate(offset);

						if (guiNode->canvas){
							guiNode->canvas->translate(offset);
						}
						if (guiNode->label){
							guiNode->label->translate(offset);
						}
					}

					for (auto& inlet : node->inlets){
						inlet.translate(offset);
					}
					for (auto& outlet : node->outlets){
						outlet.translate(offset);
					}
				}
			}
		}
	}
	else if (guiMsg.command == "gui_canvas_move_selection"){

		if (auto canvas = this->getCanvas(guiMsg.canvasId)){

			guiMsg.parseRect(1);

			canvas->region.set(guiMsg);
			canvas->mode = PdCanvas::MODE_REGION;
		}
	}
	else if (guiMsg.command == "gui_canvas_hide_selection"){

		if (auto canvas = this->getCanvas(guiMsg.canvasId)){
			canvas->mode = PdCanvas::MODE_NONE;
		}
	}
	else {
		// ofLogVerbose("pd") << aMsg;
	}
}


/*
[verbose] pd: gui_undo_menu "x268b500","no","no"
[verbose] pd: gui_undo_menu "x268cc50","no","no"
[verbose] pd: gui_undo_menu "x26b2590","no","no"
[verbose] pd: gui_undo_menu "x26f43a0","no","no"
[verbose] pd: gui_undo_menu "x26fedc0","no","no"
[verbose] pd: gui_undo_menu "x2702240","no","no"
[verbose] pd: gui_canvas_new "x268b500",1138,653,"+118+152",0,"main.pd","/developer/OF/v0.9.7/apps/pd/mPD/bin/data",0,""
[verbose] pd: gui_canvas_new "x268b500",1138,653,"+118+152",0,"main.pd","/developer/OF/v0.9.7/apps/pd/mPD/bin/data",0,""gui_canvas_new "x268b500",1138,653,"+118+152",0,"main.pd","/developer/OF/v0.9.7/apps/pd/mPD/bin/data",0,""
[verbose] pd: gui_set_toplevel_window_list "dummy",["main.pd","x268b500"]
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t271e030","graph",67,38,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t271e030","none",0,67,38,267,178
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t271c340","atom",69,40,0
[verbose] pd: gui_text_set "x268b500",".x268b500.t271c340","0"
[verbose] pd: gui_atom_draw_border "x268b500",".x268b500.t271c340",152,13
[verbose] pd: gui_text_new "x268b500",".x268b500.t271c340","atom",0,2,10,"0",5
[verbose] pd: gui_gobj_new "x268b500","x26ae8c0","iemgui",76,103,0
[verbose] pd: gui_text_draw_border "x268b500","x26ae8c0","#fcfcfc",0,76,103,91,118
[verbose] pd: gui_iemgui_base_color "x268b500","x26ae8c0","xfcfcfc"
[verbose] pd: gui_toggle_new "x268b500","x26ae8c0","x000000",1,0,78,105,89,116,78,116,89,105,76,103
[verbose] pd: gui_iemgui_label_new "x268b500","x26ae8c0",17,7,"#000000","","DejaVu Sans Mono","normal",10
[verbose] pd: gui_gobj_new "x268b500","x26b0d50","iemgui",188,106,0
[verbose] pd: gui_text_draw_border "x268b500","x26b0d50","#fcfcfc",0,188,106,203,169
[verbose] pd: gui_iemgui_base_color "x268b500","x26b0d50","xfcfcfc"
[verbose] pd: gui_slider_new "x268b500","x26b0d50","x000000",190,166,201,166,188,106
[verbose] pd: gui_iemgui_label_new "x268b500","x26b0d50",0,-9,"#000000","","DejaVu Sans Mono","normal",10
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t26e30a0","atom",69,59,0
[verbose] pd: gui_text_set "x268b500",".x268b500.t26e30a0","0"
[verbose] pd: gui_atom_draw_border "x268b500",".x268b500.t26e30a0",152,13
[verbose] pd: gui_text_new "x268b500",".x268b500.t26e30a0","atom",0,2,10,"0",5
[verbose] pd: gui_canvas_get_scroll "x268b500"
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t271e440","obj",70,292,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t271e440","none",1,70,292,152,306
[verbose] pd: gui_text_new "x268b500",".x268b500.t271e440","obj",0,2,10,"xypad 150 150",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t271e890","graph",477,10,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t271e890","none",0,477,10,677,150
[verbose] pd: gui_graph_label "x268b500",".x268b500.t271e890",0,10,"array1","DejaVu Sans Mono",5,"normal",0
[verbose] pd: gui_scalar_new "x268b500","scalar26b4318",0,2,0,0,-70,477,80,0
[verbose] pd: gui_scalar_draw_group "x268b500","dgroup25f4b90.26b4318","scalar26b4318gobj"
[verbose] pd: gui_plot_vis "x268b500",0,6,["M",0,-0.919926,"H",1,"V",-0.948497,"H",0,"z","M",1,-0.963974,"H",2,"V",-0.992545,"H",1,"z","M",2,-0.965164,"H",3,"V",-0.993735,"H",2,"z","M",3,-0.966355,"H",4,"V",-0.994926,"H",3,"z","M",4,-0.967545,"H",5,"V",-0.996116,"H",4,"z","M",5,-0.968736,"H",6,"V",-0.997307,"H",5,"z","M",6,-0.969926,"H",7,"V",-0.998497,"H",6,"z","M",7,-0.971117,"H",8,"V",-0.999688,"H",7,"z","M",8,-0.972307,"H",9,"V",-1.00088,"H",8,"z","M",9,-0.973498,"H",10,"V",-1.00207,"H",9,"z","M",10,-0.974688,"H",11,"V",-1.00326,"H",10,"z","M",11,-0.975879,"H",12,"V",-1.00445,"H",11,"z","M",12,-0.977069,"H",13,"V",-1.00564,"H",12,"z","M",13,-0.97764,"H",14,"V",-1.00621,"H",13,"z","M",14,-0.978212,"H",15,"V",-1.00678,"H",14,"z","M",15,-0.978783,"H",16,"V",-1.00735,"H",15,"z","M",16,-0.979355,"H",17,"V",-1.00793,"H",16,"z","M",17,-0.879926,"H",18,"V",-0.908497,"H",17,"z","M",18,-0.837069,"H",19,"V",-0.86564,"H",18,"z","M",19,-0.801355,"H",20,"V",-0.829926,"H",19,"z","M",20,-0.765641,"H",21,"V",-0.794212,"H",20,"z","M",21,-0.746593,"H",22,"V",-0.775164,"H",21,"z","M",22,-0.727546,"H",23,"V",-0.756117,"H",22,"z","M",23,-0.708498,"H",24,"V",-0.737069,"H",23,"z","M",24,-0.684689,"H",25,"V",-0.71326,"H",24,"z","M",25,-0.660879,"H",26,"V",-0.68945,"H",25,"z","M",26,-0.460744,"H",27,"V",-0.489315,"H",26,"z","M",27,-0.460744,"H",28,"V",-0.489315,"H",27,"z","M",28,-0.389315,"H",29,"V",-0.417886,"H",28,"z","M",29,-0.317887,"H",30,"V",-0.346458,"H",29,"z","M",30,-0.246459,"H",31,"V",-0.27503,"H",30,"z","M",31,-0.217887,"H",32,"V",-0.246458,"H",31,"z","M",32,-0.189316,"H",33,"V",-0.217887,"H",32,"z","M",33,-0.17503,"H",34,"V",-0.203601,"H",33,"z","M",34,-0.660747,"H",35,"V",-0.689318,"H",34,"z","M",35,-0.603603,"H",36,"V",-0.632174,"H",35,"z","M",36,-0.560746,"H",37,"V",-0.589317,"H",36,"z","M",37,-0.532174,"H",38,"V",-0.560745,"H",37,"z","M",38,-0.517889,"H",39,"V",-0.54646,"H",38,"z","M",39,-0.503603,"H",40,"V",-0.532174,"H",39,"z","M",40,-0.44646,"H",41,"V",-0.475031,"H",40,"z","M",41,-0.427412,"H",42,"V",-0.455983,"H",41,"z","M",42,-0.408364,"H",43,"V",-0.436935,"H",42,"z","M",43,-0.389316,"H",44,"V",-0.417887,"H",43,"z","M",44,-0.367888,"H",45,"V",-0.396459,"H",44,"z","M",45,-0.346459,"H",46,"V",-0.37503,"H",45,"z","M",46,-0.322649,"H",47,"V",-0.35122,"H",46,"z","M",47,-0.29884,"H",48,"V",-0.327411,"H",47,"z","M",48,-0.27503,"H",49,"V",-0.303601,"H",48,"z","M",49,-0.260744,"H",50,"V",-0.289315,"H",49,"z","M",50,-0.246458,"H",51,"V",-0.275029,"H",50,"z","M",51,-0.232173,"H",52,"V",-0.260744,"H",51,"z","M",52,-0.217887,"H",53,"V",-0.246458,"H",52,"z","M",53,-0.203601,"H",54,"V",-0.232172,"H",53,"z","M",54,-0.203601,"H",55,"V",-0.232172,"H",54,"z","M",55,-0.203601,"H",56,"V",-0.232172,"H",55,"z","M",56,-0.203601,"H",57,"V",-0.232172,"H",56,"z","M",57,-0.189316,"H",58,"V",-0.217887,"H",57,"z","M",58,-0.17503,"H",59,"V",-0.203601,"H",58,"z","M",59,-0.17503,"H",60,"V",-0.203601,"H",59,"z","M",60,-0.160744,"H",61,"V",-0.189315,"H",60,"z","M",61,-0.160744,"H",62,"V",-0.189315,"H",61,"z","M",62,-0.117887,"H",63,"V",-0.146458,"H",62,"z","M",63,-0.0607446,"H",64,"V",-0.089316,"H",63,"z","M",64,-0.0321732,"H",65,"V",-0.0607446,"H",64,"z","M",65,0.0535409,"H",66,"V",0.0249695,"H",65,"z","M",66,0.139255,"H",67,"V",0.110684,"H",66,"z","M",67,0.196398,"H",68,"V",0.167827,"H",67,"z","M",68,0.25354,"H",69,"V",0.224969,"H",68,"z","M",69,0.324969,"H",70,"V",0.296398,"H",69,"z","M",70,0.382112,"H",71,"V",0.353541,"H",70,"z","M",71,0.477009,"H",72,"V",0.448438,"H",71,"z","M",72,0.500479,"H",73,"V",0.471908,"H",72,"z","M",73,0.523948,"H",74,"V",0.495377,"H",73,"z","M",74,0.547417,"H",75,"V",0.518846,"H",74,"z","M",75,0.570887,"H",76,"V",0.542316,"H",75,"z","M",76,0.594356,"H",77,"V",0.565785,"H",76,"z","M",77,0.617825,"H",78,"V",0.589254,"H",77,"z","M",78,0.641295,"H",79,"V",0.612724,"H",78,"z","M",79,0.664764,"H",80,"V",0.636193,"H",79,"z","M",80,0.688233,"H",81,"V",0.659662,"H",80,"z","M",81,0.711703,"H",82,"V",0.683132,"H",81,"z","M",82,0.735172,"H",83,"V",0.706601,"H",82,"z","M",83,0.758641,"H",84,"V",0.73007,"H",83,"z","M",84,0.772927,"H",85,"V",0.744356,"H",84,"z","M",85,0.787213,"H",86,"V",0.758642,"H",85,"z","M",86,0.801498,"H",87,"V",0.772927,"H",86,"z","M",87,0.815784,"H",88,"V",0.787213,"H",87,"z","M",88,0.83007,"H",89,"V",0.801499,"H",88,"z","M",89,0.844355,"H",90,"V",0.815784,"H",89,"z","M",90,0.858641,"H",91,"V",0.83007,"H",90,"z","M",91,0.872927,"H",92,"V",0.844356,"H",91,"z","M",92,0.887212,"H",93,"V",0.858641,"H",92,"z","M",93,0.901498,"H",94,"V",0.872927,"H",93,"z","M",94,0.907212,"H",95,"V",0.878641,"H",94,"z","M",95,0.912926,"H",96,"V",0.884355,"H",95,"z","M",96,0.918641,"H",97,"V",0.89007,"H",96,"z","M",97,0.924355,"H",98,"V",0.895784,"H",97,"z","M",98,0.930069,"H",99,"V",0.901498,"H",98,"z","M",99,0.787213,"H",100,"V",0.758642,"H",99,"z"],["fill","black","stroke","black","stroke-width",0],["dgroup25f4b90.26b4318","dgroup25f4b90.26b4318"]
[verbose] pd: gui_find_lowest_and_arrange "x268b500",".x268b500.t271eca0",".x268b500.x26b2590.template26b4318"
[verbose] pd: gui_canvas_get_scroll "x268b500"
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t271eca0","obj",241,227,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t271eca0","none",0,241,227,299,241
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t271eca0",".x268b500.t271eca0",241,238,248,241,241,227,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t271eca0",".x268b500.t271eca0",258,238,265,241,241,227,"o",1,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t271eca0",".x268b500.t271eca0",275,238,282,241,241,227,"o",2,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t271eca0",".x268b500.t271eca0",292,238,299,241,241,227,"o",3,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t271eca0",".x268b500.t271eca0",241,227,248,230,241,227,"i",0,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t271eca0","obj",0,2,10,"t f b b f",5
[verbose] pd: gui_gobj_new "x268b500","x26b84e0","iemgui",49,30,1
[verbose] pd: gui_text_draw_border "x268b500","x26b84e0","#fcb8d4",0,49,30,64,263
[verbose] pd: gui_iemgui_base_color "x268b500","x26b84e0","xfcb8d4"
[verbose] pd: gui_slider_new "x268b500","x26b84e0","x000000",51,260,62,260,49,30
[verbose] pd: gui_iemgui_label_new "x268b500","x26b84e0",0,-9,"#000000","","DejaVu Sans Mono","normal",10
[verbose] pd: gui_gobj_draw_io "x268b500","x26b84e0",".x268b500.t271f0d0o0",49,262,56,263,49,30,"o",0,0,1
[verbose] pd: gui_gobj_draw_io "x268b500","x26b84e0",".x268b500.t271f0d0i0",49,30,56,31,49,30,"i",0,0,1
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2688840","atom",277,209,1
[verbose] pd: gui_text_set "x268b500",".x268b500.t2688840","0"
[verbose] pd: gui_atom_draw_border "x268b500",".x268b500.t2688840",56,13
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2688840",".x268b500.t2688840",277,219,284,222,277,209,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2688840",".x268b500.t2688840",277,209,284,212,277,209,"i",0,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t2688840","atom",0,2,10,"0",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2688c30","obj",70,272,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t2688c30","none",1,70,272,134,286
[verbose] pd: gui_text_new "x268b500",".x268b500.t2688c30","obj",0,2,10,"bkablabdsa",5
[verbose] pd: gui_gobj_new "x268b500","x26bac50","iemgui",55,316,1
[verbose] pd: gui_text_draw_border "x268b500","x26bac50","#fcfcfc",0,55,316,70,436
[verbose] pd: gui_iemgui_base_color "x268b500","x26bac50","xfcfcfc"
[verbose] pd: gui_radio_create_buttons "x268b500","x26bac50","x000000",58,319,67,328,55,316,0,1
[verbose] pd: gui_radio_new "x268b500","x26bac50",55,331,70,331,1,55,316
[verbose] pd: gui_radio_create_buttons "x268b500","x26bac50","x000000",58,334,67,343,55,316,1,0
[verbose] pd: gui_radio_new "x268b500","x26bac50",55,346,70,346,2,55,316
[verbose] pd: gui_radio_create_buttons "x268b500","x26bac50","x000000",58,349,67,358,55,316,2,0
[verbose] pd: gui_radio_new "x268b500","x26bac50",55,361,70,361,3,55,316
[verbose] pd: gui_radio_create_buttons "x268b500","x26bac50","x000000",58,364,67,373,55,316,3,0
[verbose] pd: gui_radio_new "x268b500","x26bac50",55,376,70,376,4,55,316
[verbose] pd: gui_radio_create_buttons "x268b500","x26bac50","x000000",58,379,67,388,55,316,4,0
[verbose] pd: gui_radio_new "x268b500","x26bac50",55,391,70,391,5,55,316
[verbose] pd: gui_radio_create_buttons "x268b500","x26bac50","x000000",58,394,67,403,55,316,5,0
[verbose] pd: gui_radio_new "x268b500","x26bac50",55,406,70,406,6,55,316
[verbose] pd: gui_radio_create_buttons "x268b500","x26bac50","x000000",58,409,67,418,55,316,6,0
[verbose] pd: gui_radio_new "x268b500","x26bac50",55,421,70,421,7,55,316
[verbose] pd: gui_radio_create_buttons "x268b500","x26bac50","x000000",58,424,67,433,55,316,7,0
[verbose] pd: gui_iemgui_label_new "x268b500","x26bac50",0,-8,"#000000","","DejaVu Sans Mono","normal",10
[verbose] pd: gui_gobj_draw_io "x268b500","x26bac50",".x268b500.t2689060o0",55,435,62,436,55,316,"o",0,0,1
[verbose] pd: gui_gobj_draw_io "x268b500","x26bac50",".x268b500.t2689060i0",55,316,62,317,55,316,"i",0,0,1
[verbose] pd: gui_gobj_new "x268b500","x26f3070","iemgui",75,361,1
[verbose] pd: gui_text_draw_border "x268b500","x26f3070","#fcfcfc",0,75,361,195,376
[verbose] pd: gui_iemgui_base_color "x268b500","x26f3070","xfcfcfc"
[verbose] pd: gui_radio_create_buttons "x268b500","x26f3070","xf8fc00",78,364,87,373,75,361,0,1
[verbose] pd: gui_radio_new "x268b500","x26f3070",90,361,90,376,1,75,361
[verbose] pd: gui_radio_create_buttons "x268b500","x26f3070","xf8fc00",93,364,102,373,75,361,1,0
[verbose] pd: gui_radio_new "x268b500","x26f3070",105,361,105,376,2,75,361
[verbose] pd: gui_radio_create_buttons "x268b500","x26f3070","xf8fc00",108,364,117,373,75,361,2,0
[verbose] pd: gui_radio_new "x268b500","x26f3070",120,361,120,376,3,75,361
[verbose] pd: gui_radio_create_buttons "x268b500","x26f3070","xf8fc00",123,364,132,373,75,361,3,0
[verbose] pd: gui_radio_new "x268b500","x26f3070",135,361,135,376,4,75,361
[verbose] pd: gui_radio_create_buttons "x268b500","x26f3070","xf8fc00",138,364,147,373,75,361,4,0
[verbose] pd: gui_radio_new "x268b500","x26f3070",150,361,150,376,5,75,361
[verbose] pd: gui_radio_create_buttons "x268b500","x26f3070","xf8fc00",153,364,162,373,75,361,5,0
[verbose] pd: gui_radio_new "x268b500","x26f3070",165,361,165,376,6,75,361
[verbose] pd: gui_radio_create_buttons "x268b500","x26f3070","xf8fc00",168,364,177,373,75,361,6,0
[verbose] pd: gui_radio_new "x268b500","x26f3070",180,361,180,376,7,75,361
[verbose] pd: gui_radio_create_buttons "x268b500","x26f3070","xf8fc00",183,364,192,373,75,361,7,0
[verbose] pd: gui_iemgui_label_new "x268b500","x26f3070",0,-8,"#000000","fd_ddff","DejaVu Sans Mono","normal",10
[verbose] pd: gui_gobj_draw_io "x268b500","x26f3070",".x268b500.t2689620o0",75,375,82,376,75,361,"o",0,0,1
[verbose] pd: gui_gobj_draw_io "x268b500","x26f3070",".x268b500.t2689620i0",75,361,82,362,75,361,"i",0,0,1
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2689c10","graph",415,236,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t2689c10","none",0,415,236,615,376
[verbose] pd: gui_graph_label "x268b500",".x268b500.t2689c10",0,10,"array2","DejaVu Sans Mono",5,"normal",0
[verbose] pd: gui_scalar_new "x268b500","scalar26f58d8",0,66.6667,0,0,-70,415,306,0
[verbose] pd: gui_scalar_draw_group "x268b500","dgroup25f4b90.26f58d8","scalar26f58d8gobj"
[verbose] pd: gui_plot_vis "x268b500",0,6,["M",0,-0.342859,"H",1,"V",-0.37143,"H",0,"z","M",1,0.185715,"H",2,"V",0.157144,"H",1,"z","M",2,0.285716,"H",3,"V",0.257145,"H",2,"z"],["fill","black","stroke","black","stroke-width",0],["dgroup25f4b90.26f58d8","dgroup25f4b90.26f58d8"]
[verbose] pd: gui_find_lowest_and_arrange "x268b500",".x268b500.t268a170",".x268b500.x26f43a0.template26f58d8"
[verbose] pd: gui_canvas_get_scroll "x268b500"
[verbose] pd: gui_gobj_new "x268b500","x26f7420","iemgui",292,190,1
[verbose] pd: gui_text_draw_border "x268b500","x26f7420","#fcfcfc",0,292,190,307,205
[verbose] pd: gui_iemgui_base_color "x268b500","x26f7420","xfcfcfc"
[verbose] pd: gui_bng_new "x268b500","x26f7420",7.5,7.5,6.5
[verbose] pd: gui_iemgui_label_new "x268b500","x26f7420",17,7,"#000000","","DejaVu Sans Mono","normal",10
[verbose] pd: gui_gobj_draw_io "x268b500","x26f7420",".x268b500.t268a170o0",292,204,299,205,292,190,"o",0,0,1
[verbose] pd: gui_gobj_draw_io "x268b500","x26f7420",".x268b500.t268a170i0",292,190,299,191,292,190,"i",0,0,1
[verbose] pd: gui_gobj_new "x268b500","x26f9280","iemgui",317,190,1
[verbose] pd: gui_text_draw_border "x268b500","x26f9280","#fcfcfc",0,317,190,332,205
[verbose] pd: gui_iemgui_base_color "x268b500","x26f9280","xfcfcfc"
[verbose] pd: gui_toggle_new "x268b500","x26f9280","xfc0400",1,0,319,192,330,203,319,203,330,192,317,190
[verbose] pd: gui_iemgui_label_new "x268b500","x26f9280",17,7,"#000000","","DejaVu Sans Mono","normal",10
[verbose] pd: gui_gobj_draw_io "x268b500","x26f9280",".x268b500.t268a720o0",317,204,324,205,317,190,"o",0,0,1
[verbose] pd: gui_gobj_draw_io "x268b500","x26f9280",".x268b500.t268a720i0",317,190,324,191,317,190,"i",0,0,1
[verbose] pd: gui_gobj_new "x268b500","x26fb710","iemgui",292,393,1
[verbose] pd: gui_text_draw_border "x268b500","x26fb710","#fcfcfc",0,292,393,425,408
[verbose] pd: gui_iemgui_base_color "x268b500","x26fb710","xfcfcfc"
[verbose] pd: gui_slider_new "x268b500","x26fb710","x000000",295,395,295,406,292,393
[verbose] pd: gui_iemgui_label_new "x268b500","x26fb710",-2,-8,"#000000","ggggg_dfs","DejaVu Sans Mono","normal",10
[verbose] pd: gui_gobj_draw_io "x268b500","x26fb710",".x268b500.t268ace0o0",292,407,299,408,292,393,"o",0,0,1
[verbose] pd: gui_gobj_draw_io "x268b500","x26fb710",".x268b500.t268ace0i0",292,393,299,394,292,393,"i",0,0,1
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2713780","obj",207,371,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t2713780","none",0,207,371,235,385
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2713780",".x268b500.t2713780",207,382,214,385,207,371,"o",0,1,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2713780",".x268b500.t2713780",207,371,214,374,207,371,"i",0,1,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2713780",".x268b500.t2713780",228,371,235,374,207,371,"i",1,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t2713780","obj",0,2,10,"osc~",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2713b80","obj",241,209,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t2713b80","none",0,241,209,275,223
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2713b80",".x268b500.t2713b80",241,209,248,212,241,209,"i",0,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t2713b80","obj",0,2,10,"print",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2713f90","atom",284,88,1
[verbose] pd: gui_text_set "x268b500",".x268b500.t2713f90","0"
[verbose] pd: gui_atom_draw_border "x268b500",".x268b500.t2713f90",32,13
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2713f90",".x268b500.t2713f90",284,98,291,101,284,88,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2713f90",".x268b500.t2713f90",284,88,291,91,284,88,"i",0,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t2713f90","atom",0,2,10,"0",5
[verbose] pd: gui_gobj_new "x268b500","x26fdcb0","iemgui",267,65,1
[verbose] pd: gui_text_draw_border "x268b500","x26fdcb0","#00fc04",0,267,65,282,80
[verbose] pd: gui_iemgui_base_color "x268b500","x26fdcb0","x00fc04"
[verbose] pd: gui_toggle_new "x268b500","x26fdcb0","x00fc04",1,0,269,67,280,78,269,78,280,67,267,65
[verbose] pd: gui_iemgui_label_new "x268b500","x26fdcb0",17,7,"#000000","","DejaVu Sans Mono","normal",10
[verbose] pd: gui_gobj_draw_io "x268b500","x26fdcb0",".x268b500.t2714380o0",267,79,274,80,267,65,"o",0,0,1
[verbose] pd: gui_gobj_draw_io "x268b500","x26fdcb0",".x268b500.t2714380i0",267,65,274,66,267,65,"i",0,0,1
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2714920","graph",737,10,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t2714920","none",0,737,10,937,150
[verbose] pd: gui_graph_label "x268b500",".x268b500.t2714920",0,10,"array1","DejaVu Sans Mono",5,"normal",0
[verbose] pd: gui_scalar_new "x268b500","scalar27000b8",0,2,0,0,-70,737,80,0
[verbose] pd: gui_scalar_draw_group "x268b500","dgroup25f4b90.27000b8","scalar27000b8gobj"
[verbose] pd: gui_plot_vis "x268b500",0,6,["M",0,0.415785,"H",1,"V",0.387214,"H",0,"z","M",1,0.415785,"H",2,"V",0.387214,"H",1,"z","M",2,0.415785,"H",3,"V",0.387214,"H",2,"z","M",3,0.415785,"H",4,"V",0.387214,"H",3,"z","M",4,0.415785,"H",5,"V",0.387214,"H",4,"z","M",5,0.415785,"H",6,"V",0.387214,"H",5,"z","M",6,0.415785,"H",7,"V",0.387214,"H",6,"z","M",7,0.415785,"H",8,"V",0.387214,"H",7,"z","M",8,0.41059,"H",9,"V",0.382019,"H",8,"z","M",9,0.405395,"H",10,"V",0.376824,"H",9,"z","M",10,0.4002,"H",11,"V",0.371629,"H",10,"z","M",11,0.395006,"H",12,"V",0.366435,"H",11,"z","M",12,0.389811,"H",13,"V",0.36124,"H",12,"z","M",13,0.384616,"H",14,"V",0.356045,"H",13,"z","M",14,0.379421,"H",15,"V",0.35085,"H",14,"z","M",15,0.374226,"H",16,"V",0.345655,"H",15,"z","M",16,0.369032,"H",17,"V",0.340461,"H",16,"z","M",17,0.363837,"H",18,"V",0.335266,"H",17,"z","M",18,0.358642,"H",19,"V",0.330071,"H",18,"z","M",19,0.347531,"H",20,"V",0.31896,"H",19,"z","M",20,0.33642,"H",21,"V",0.307849,"H",20,"z","M",21,0.325309,"H",22,"V",0.296738,"H",21,"z","M",22,0.314198,"H",23,"V",0.285627,"H",22,"z","M",23,0.303087,"H",24,"V",0.274516,"H",23,"z","M",24,0.291975,"H",25,"V",0.263404,"H",24,"z","M",25,0.280864,"H",26,"V",0.252293,"H",25,"z","M",26,0.269753,"H",27,"V",0.241182,"H",26,"z","M",27,0.258642,"H",28,"V",0.230071,"H",27,"z","M",28,0.247928,"H",29,"V",0.219357,"H",28,"z","M",29,0.237214,"H",30,"V",0.208643,"H",29,"z","M",30,0.226499,"H",31,"V",0.197928,"H",30,"z","M",31,0.215785,"H",32,"V",0.187214,"H",31,"z","M",32,0.205071,"H",33,"V",0.1765,"H",32,"z","M",33,0.194357,"H",34,"V",0.165786,"H",33,"z","M",34,0.183642,"H",35,"V",0.155071,"H",34,"z","M",35,0.172928,"H",36,"V",0.144357,"H",35,"z","M",36,0.164357,"H",37,"V",0.135786,"H",36,"z","M",37,0.155785,"H",38,"V",0.127214,"H",37,"z","M",38,0.147214,"H",39,"V",0.118643,"H",38,"z","M",39,0.138643,"H",40,"V",0.110072,"H",39,"z","M",40,0.130071,"H",41,"V",0.1015,"H",40,"z","M",41,0.122928,"H",42,"V",0.0943566,"H",41,"z","M",42,0.115785,"H",43,"V",0.0872136,"H",42,"z","M",43,0.108643,"H",44,"V",0.0800716,"H",43,"z","M",44,0.1015,"H",45,"V",0.0729286,"H",44,"z","M",45,0.0919759,"H",46,"V",0.0634045,"H",45,"z","M",46,0.0824522,"H",47,"V",0.0538808,"H",46,"z","M",47,0.0729284,"H",48,"V",0.044357,"H",47,"z","M",48,0.0634046,"H",49,"V",0.0348332,"H",48,"z","M",49,0.0538808,"H",50,"V",0.0253094,"H",49,"z","M",50,0.044357,"H",51,"V",0.0157856,"H",50,"z","M",51,0.0372142,"H",52,"V",0.00864277,"H",51,"z","M",52,0.0300713,"H",53,"V",0.00149987,"H",52,"z","M",53,0.0229285,"H",54,"V",-0.00564293,"H",53,"z","M",54,0.0157856,"H",55,"V",-0.0127858,"H",54,"z","M",55,0.0538808,"H",56,"V",0.0253094,"H",55,"z","M",56,0.0919759,"H",57,"V",0.0634045,"H",56,"z","M",57,0.130071,"H",58,"V",0.1015,"H",57,"z","M",58,0.155071,"H",59,"V",0.1265,"H",58,"z","M",59,0.180071,"H",60,"V",0.1515,"H",59,"z","M",60,0.205071,"H",61,"V",0.1765,"H",60,"z","M",61,0.230071,"H",62,"V",0.2015,"H",61,"z","M",62,0.255071,"H",63,"V",0.2265,"H",62,"z","M",63,0.280071,"H",64,"V",0.2515,"H",63,"z","M",64,0.305071,"H",65,"V",0.2765,"H",64,"z","M",65,0.330071,"H",66,"V",0.3015,"H",65,"z","M",66,0.355071,"H",67,"V",0.3265,"H",66,"z","M",67,0.380071,"H",68,"V",0.3515,"H",67,"z","M",68,0.405071,"H",69,"V",0.3765,"H",68,"z","M",69,0.430071,"H",70,"V",0.4015,"H",69,"z","M",70,0.45354,"H",71,"V",0.424969,"H",70,"z","M",71,0.477009,"H",72,"V",0.448438,"H",71,"z","M",72,0.500479,"H",73,"V",0.471908,"H",72,"z","M",73,0.523948,"H",74,"V",0.495377,"H",73,"z","M",74,0.547417,"H",75,"V",0.518846,"H",74,"z","M",75,0.570887,"H",76,"V",0.542316,"H",75,"z","M",76,0.594356,"H",77,"V",0.565785,"H",76,"z","M",77,0.617825,"H",78,"V",0.589254,"H",77,"z","M",78,0.641295,"H",79,"V",0.612724,"H",78,"z","M",79,0.664764,"H",80,"V",0.636193,"H",79,"z","M",80,0.688233,"H",81,"V",0.659662,"H",80,"z","M",81,0.711703,"H",82,"V",0.683132,"H",81,"z","M",82,0.735172,"H",83,"V",0.706601,"H",82,"z","M",83,0.758641,"H",84,"V",0.73007,"H",83,"z","M",84,0.772927,"H",85,"V",0.744356,"H",84,"z","M",85,0.787213,"H",86,"V",0.758642,"H",85,"z","M",86,0.801498,"H",87,"V",0.772927,"H",86,"z","M",87,0.815784,"H",88,"V",0.787213,"H",87,"z","M",88,0.83007,"H",89,"V",0.801499,"H",88,"z","M",89,0.844355,"H",90,"V",0.815784,"H",89,"z","M",90,0.858641,"H",91,"V",0.83007,"H",90,"z","M",91,0.872927,"H",92,"V",0.844356,"H",91,"z","M",92,0.887212,"H",93,"V",0.858641,"H",92,"z","M",93,0.901498,"H",94,"V",0.872927,"H",93,"z","M",94,0.907212,"H",95,"V",0.878641,"H",94,"z","M",95,0.912926,"H",96,"V",0.884355,"H",95,"z","M",96,0.918641,"H",97,"V",0.89007,"H",96,"z","M",97,0.924355,"H",98,"V",0.895784,"H",97,"z","M",98,0.930069,"H",99,"V",0.901498,"H",98,"z","M",99,0.787213,"H",100,"V",0.758642,"H",99,"z"],["fill","black","stroke","black","stroke-width",0],["dgroup25f4b90.27000b8","dgroup25f4b90.27000b8"]
[verbose] pd: gui_find_lowest_and_arrange "x268b500",".x268b500.t2714d30",".x268b500.x26fedc0.template27000b8"
[verbose] pd: gui_canvas_get_scroll "x268b500"
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2714d30","graph",737,170,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t2714d30","none",0,737,170,937,310
[verbose] pd: gui_graph_label "x268b500",".x268b500.t2714d30",0,10,"array2","DejaVu Sans Mono",5,"normal",0
[verbose] pd: gui_scalar_new "x268b500","scalar2703d28",0,33.3333,0,0,-70,737,240,0
[verbose] pd: gui_scalar_draw_group "x268b500","dgroup25f4b90.2703d28","scalar2703d28gobj"
[verbose] pd: gui_plot_vis "x268b500",0,6,["M",0,-0.342859,"H",1,"V",-0.37143,"H",0,"z","M",1,0.185715,"H",2,"V",0.157144,"H",1,"z","M",2,0.285716,"H",3,"V",0.257145,"H",2,"z","M",3,0.342859,"H",4,"V",0.314288,"H",3,"z","M",4,0.671433,"H",5,"V",0.642862,"H",4,"z","M",5,-0.800005,"H",6,"V",-0.828576,"H",5,"z"],["fill","black","stroke","black","stroke-width",0],["dgroup25f4b90.2703d28","dgroup25f4b90.2703d28"]
[verbose] pd: gui_find_lowest_and_arrange "x268b500",".x268b500.t2715140",".x268b500.x2702240.template2703d28"
[verbose] pd: gui_canvas_get_scroll "x268b500"
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2715140","obj",284,113,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t2715140","none",0,284,113,324,127
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2715140",".x268b500.t2715140",284,124,291,127,284,113,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2715140",".x268b500.t2715140",284,113,291,116,284,113,"i",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2715140",".x268b500.t2715140",317,113,324,116,284,113,"i",1,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t2715140","obj",0,2,10,"* 1000",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2715550","atom",168,228,1
[verbose] pd: gui_text_set "x268b500",".x268b500.t2715550","0"
[verbose] pd: gui_atom_draw_border "x268b500",".x268b500.t2715550",32,13
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2715550",".x268b500.t2715550",168,238,175,241,168,228,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2715550",".x268b500.t2715550",168,228,175,231,168,228,"i",0,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t2715550","atom",0,2,10,"0",5
[verbose] pd: gui_text_new "x268b500",".x268b500.t2715550","gatom",0,-141,9,"Test_let's_see_how_long",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2715940","msg",342,427,1
[verbose] pd: gui_message_draw_border "x268b500",".x268b500.t2715940",52,14
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2715940",".x268b500.t2715940",342,438,349,441,342,427,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2715940",".x268b500.t2715940",342,427,349,430,342,427,"i",0,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t2715940","msg",0,2,10,"blab bla",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2715d60","obj",367,140,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t2715d60","none",0,367,140,401,154
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2715d60",".x268b500.t2715d60",367,151,374,154,367,140,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2715d60",".x268b500.t2715d60",367,140,374,143,367,140,"i",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2715d60",".x268b500.t2715d60",394,140,401,143,367,140,"i",1,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t2715d60","obj",0,2,10,"until",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2716170","obj",160,440,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t2716170","none",0,160,440,194,454
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2716170",".x268b500.t2716170",160,440,167,443,160,440,"i",0,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t2716170","obj",0,2,10,"print",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2716580","obj",256,393,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t2716580","none",0,256,393,284,407
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2716580",".x268b500.t2716580",256,404,263,407,256,393,"o",0,1,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2716580",".x268b500.t2716580",256,393,263,396,256,393,"i",0,1,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2716580",".x268b500.t2716580",277,393,284,396,256,393,"i",1,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t2716580","obj",0,2,10,"osc~",5
[verbose] pd: gui_gobj_new "x268b500","x2707cd0","iemgui",81,190,1
[verbose] pd: gui_text_draw_border "x268b500","x2707cd0","#fcfcfc",0,81,190,96,205
[verbose] pd: gui_iemgui_base_color "x268b500","x2707cd0","xfcfcfc"
[verbose] pd: gui_bng_new "x268b500","x2707cd0",7.5,7.5,6.5
[verbose] pd: gui_iemgui_label_new "x268b500","x2707cd0",17,7,"#000000","","DejaVu Sans Mono","normal",10
[verbose] pd: gui_gobj_draw_io "x268b500","x2707cd0",".x268b500.t2716980o0",81,204,88,205,81,190,"o",0,0,1
[verbose] pd: gui_gobj_draw_io "x268b500","x2707cd0",".x268b500.t2716980i0",81,190,88,191,81,190,"i",0,0,1
[verbose] pd: gui_gobj_new "x268b500","x2709b90","iemgui",106,190,1
[verbose] pd: gui_text_draw_border "x268b500","x2709b90","#fcfcfc",0,106,190,121,205
[verbose] pd: gui_iemgui_base_color "x268b500","x2709b90","xfcfcfc"
[verbose] pd: gui_toggle_new "x268b500","x2709b90","x000000",1,0,108,192,119,203,108,203,119,192,106,190
[verbose] pd: gui_iemgui_label_new "x268b500","x2709b90",17,7,"#000000","fff_dds__a_aa","DejaVu Sans Mono","normal",10
[verbose] pd: gui_gobj_draw_io "x268b500","x2709b90",".x268b500.t2716f30o0",106,204,113,205,106,190,"o",0,0,1
[verbose] pd: gui_gobj_draw_io "x268b500","x2709b90",".x268b500.t2716f30i0",106,190,113,191,106,190,"i",0,0,1
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2717500","msg",78,326,1
[verbose] pd: gui_message_draw_border "x268b500",".x268b500.t2717500",52,14
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2717500",".x268b500.t2717500",78,337,85,340,78,326,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2717500",".x268b500.t2717500",78,326,85,329,78,326,"i",0,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t2717500","msg",0,2,10,"blab bla",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2717920","atom",154,320,1
[verbose] pd: gui_text_set "x268b500",".x268b500.t2717920","0"
[verbose] pd: gui_atom_draw_border "x268b500",".x268b500.t2717920",32,13
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2717920",".x268b500.t2717920",154,330,161,333,154,320,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2717920",".x268b500.t2717920",154,320,161,323,154,320,"i",0,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t2717920","atom",0,2,10,"0",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2717d10","atom",366,451,1
[verbose] pd: gui_text_set "x268b500",".x268b500.t2717d10","0"
[verbose] pd: gui_atom_draw_border "x268b500",".x268b500.t2717d10",32,13
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2717d10",".x268b500.t2717d10",366,461,373,464,366,451,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2717d10",".x268b500.t2717d10",366,451,373,454,366,451,"i",0,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t2717d10","atom",0,2,10,"0",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2718100","obj",153,291,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t2718100","none",0,153,291,193,305
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2718100",".x268b500.t2718100",153,302,160,305,153,291,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2718100",".x268b500.t2718100",153,291,160,294,153,291,"i",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2718100",".x268b500.t2718100",186,291,193,294,153,291,"i",1,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t2718100","obj",0,2,10,"* 1000",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2718510","comment",226,255,1
[verbose] pd: gui_text_new "x268b500",".x268b500.t2718510","text",0,2,10,"This is a comment, apparently dsdsd dsdsd ds d dsdds dsdd
dsds",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2718ae0","obj",457,424,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t2718ae0","none",1,457,424,701,438
[verbose] pd: gui_text_new "x268b500",".x268b500.t2718ae0","obj",0,2,10,"comment 0 10 helvetica ? 0 0 0 0 comment",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2719000","obj",472,496,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t2719000","none",1,472,496,716,510
[verbose] pd: gui_text_new "x268b500",".x268b500.t2719000","obj",0,2,10,"comment 0 10 helvetica ? 0 0 0 0 comment",5
[verbose] pd: gui_numbox_new "x268b500","x270f100","xfcfcfc",168,248,54,14,1
[verbose] pd: gui_numbox_draw_text "x268b500","x270f100","0",10,"x000000",177,256,168,248
[verbose] pd: gui_iemgui_label_new "x268b500","x270f100",0,-8,"#000000","","DejaVu Sans Mono","normal",10
[verbose] pd: gui_gobj_draw_io "x268b500","x270f100",".x268b500.t2719520o0",168,261,175,262,168,248,"o",0,0,1
[verbose] pd: gui_gobj_draw_io "x268b500","x270f100",".x268b500.t2719520i0",168,248,175,249,168,248,"i",0,0,1
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2719b50","atom",87,242,1
[verbose] pd: gui_text_set "x268b500",".x268b500.t2719b50","symbol"
[verbose] pd: gui_atom_draw_border "x268b500",".x268b500.t2719b50",62,13
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2719b50",".x268b500.t2719b50",87,252,94,255,87,242,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t2719b50",".x268b500.t2719b50",87,242,94,245,87,242,"i",0,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t2719b50","atom",0,2,10,"symbol",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t2719f60","obj",284,11,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t2719f60","none",1,284,11,342,25
[verbose] pd: gui_text_new "x268b500",".x268b500.t2719f60","obj",0,2,10,"ezoutput~",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t271a390","obj",742,381,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t271a390","none",1,742,381,806,395
[verbose] pd: gui_text_new "x268b500",".x268b500.t271a390","obj",0,2,10,"noisegate~",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t271a7c0","obj",337,72,1
[verbose] pd: gui_text_draw_border "x268b500",".x268b500.t271a7c0","none",1,337,72,431,86
[verbose] pd: gui_text_new "x268b500",".x268b500.t271a7c0","obj",0,2,10,"for++ 50 40 100",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t271ac20","msg",167,478,1
[verbose] pd: gui_message_draw_border "x268b500",".x268b500.t271ac20",364,94
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t271ac20",".x268b500.t271ac20",167,569,174,572,167,478,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t271ac20",".x268b500.t271ac20",167,478,174,481,167,478,"i",0,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t271ac20","msg",0,2,10,";
my-receiver-name ddddddddddddddd 858
fdfdfddfffffffffffffffffffffffffffffffffffffffffffffffffffff
ffffffffffffffffff;
another-receiver -45;
;
my-receiver-name;
fddfds;
fsdfs",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t271b570","msg",592,448,1
[verbose] pd: gui_message_draw_border "x268b500",".x268b500.t271b570",52,14
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t271b570",".x268b500.t271b570",592,459,599,462,592,448,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t271b570",".x268b500.t271b570",592,448,599,451,592,448,"i",0,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t271b570","msg",0,2,10,"fdffffff",5
[verbose] pd: gui_gobj_new "x268b500",".x268b500.t271b990","msg",88,215,1
[verbose] pd: gui_message_draw_border "x268b500",".x268b500.t271b990",46,14
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t271b990",".x268b500.t271b990",88,226,95,229,88,215,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x268b500",".x268b500.t271b990",".x268b500.t271b990",88,215,95,218,88,215,"i",0,0,0
[verbose] pd: gui_text_new "x268b500",".x268b500.t271b990","msg",0,2,10,"set bla",5
[verbose] pd: gui_canvas_line "x268b500","l27111d0","control",3,0,3,10,149,196,295,383,295,393
[verbose] pd: gui_canvas_line "x268b500","l2712eb0","control",3,0,3,10,80,160,157,310,157,320
[verbose] pd: gui_canvas_line "x268b500","l2712f80","signal",3,0,3,10,131,196,259,383,259,393
[verbose] pd: gui_canvas_line "x268b500","l2713050","control",3,0,3,10,145,56,287,103,287,113
[verbose] pd: gui_canvas_line "x268b500","l2713120","control",3,0,3,10,87,114,171,218,171,228
[verbose] pd: gui_canvas_line "x268b500","l27131f0","control",3,0,3,10,87,124,171,238,171,248
[verbose] pd: gui_canvas_line "x268b500","l27132c0","control",3,0,3,10,79,145,156,281,156,291
[verbose] pd: gui_canvas_line "x268b500","l2713390","control",3,0,3,10,80,160,157,310,157,320
[verbose] pd: gui_canvas_line "x268b500","l2713460","control",3,0,3,10,46,121,90,232,90,242
[verbose] pd: gui_canvas_get_scroll "x268b500"
[verbose] pd: gui_set_toplevel_window_list "dummy",["main.pd","x268b500"]
[verbose] pd: gui_pd_dsp 1
[verbose] pd: gui_canvas_cursor "x268b500","cursor_editmode_nothing"
[verbose] pd: gui_canvas_set_editmode "x268b500",1
[verbose] Pd: opening patch: data/gatom-help.pd
[verbose] pd: gui_pd_dsp 0
[verbose] pd: gui_undo_menu "x2795b20","no","no"
[verbose] pd: gui_undo_menu "x279d080","no","no"
[verbose] pd: gui_undo_menu "x27a6e30","no","no"
[verbose] pd: gui_undo_menu "x2762bd0","no","no"
[verbose] pd: gui_canvas_new "x2795b20",555,619,"+429+64",0,"gatom-help.pd","/developer/OF/v0.9.7/apps/pd/mPD/bin/data",0,""
[verbose] pd: gui_canvas_new "x2795b20",555,619,"+429+64",0,"gatom-help.pd","/developer/OF/v0.9.7/apps/pd/mPD/bin/data",0,""gui_canvas_new "x2795b20",555,619,"+429+64",0,"gatom-help.pd","/developer/OF/v0.9.7/apps/pd/mPD/bin/data",0,""
[verbose] pd: gui_set_toplevel_window_list "dummy",["gatom-help.pd","x2795b20","main.pd","x268b500"]
[verbose] pd: gui_gobj_new "x2795b20","x27986e0","iemgui",0,595,1
[verbose] pd: gui_mycanvas_new "x2795b20","x27986e0","xdcdcdc",0,595,552,616,15,610
[verbose] pd: gui_iemgui_label_new "x2795b20","x27986e0",20,12,"#404040","","DejaVu Sans Mono","normal",14
[verbose] pd: gui_gobj_new "x2795b20","x279a200","iemgui",0,0,1
[verbose] pd: gui_mycanvas_new "x2795b20","x279a200","xc4dcdc",0,0,552,40,15,15
[verbose] pd: gui_iemgui_label_new "x2795b20","x279a200",3,12,"#000000","gatom","DejaVu Sans Mono","normal",18
[verbose] pd: gui_gobj_new "x2795b20","x279bdf0","iemgui",0,231,1
[verbose] pd: gui_mycanvas_new "x2795b20","x279bdf0","xdcdcdc",0,231,550,234,3,234
[verbose] pd: gui_iemgui_label_new "x2795b20","x279bdf0",8,12,"#000000","inlets","DejaVu Sans Mono","normal",13
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t2794ed0","obj",500,597,1
[verbose] pd: gui_text_draw_border "x2795b20",".x2795b20.t2794ed0","none",0,500,597,546,611
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t2794ed0","obj",0,2,10,"pd META",5
[verbose] pd: gui_gobj_new "x2795b20","x27a23f0","iemgui",0,396,1
[verbose] pd: gui_mycanvas_new "x2795b20","x27a23f0","xdcdcdc",0,396,550,399,3,399
[verbose] pd: gui_iemgui_label_new "x2795b20","x27a23f0",8,12,"#000000","outlets","DejaVu Sans Mono","normal",13
[verbose] pd: gui_gobj_new "x2795b20","x27a3f90","iemgui",0,470,1
[verbose] pd: gui_mycanvas_new "x2795b20","x27a3f90","xdcdcdc",0,470,550,473,3,473
[verbose] pd: gui_iemgui_label_new "x2795b20","x27a3f90",8,12,"#000000","arguments","DejaVu Sans Mono","normal",13
[verbose] pd: gui_gobj_new "x2795b20","x27a5ba0","iemgui",0,499,1
[verbose] pd: gui_mycanvas_new "x2795b20","x27a5ba0","xdcdcdc",0,499,550,502,3,502
[verbose] pd: gui_iemgui_label_new "x2795b20","x27a5ba0",8,12,"#000000","more_info","DejaVu Sans Mono","normal",13
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27cf230","comment",98,474,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27cf230","text",0,2,10,"(none)",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27cf640","obj",102,597,1
[verbose] pd: gui_text_draw_border "x2795b20",".x2795b20.t27cf640","none",0,102,597,214,611
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27cf640","obj",0,2,10,"pd Related_objects",5
[verbose] pd: gui_gobj_new "x2795b20","x275c870","iemgui",78,240,1
[verbose] pd: gui_mycanvas_new "x2795b20","x275c870","xdcdcdc",78,240,81,385,95,257
[verbose] pd: gui_iemgui_label_new "x2795b20","x275c870",5,9,"#9c9c9c","0","DejaVu Sans Mono","normal",16
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d00a0","comment",98,404,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d00a0","text",0,2,10,"float",5
[verbose] pd: gui_gobj_new "x2795b20","x275e9b0","iemgui",78,405,1
[verbose] pd: gui_mycanvas_new "x2795b20","x275e9b0","xdcdcdc",78,405,81,457,95,422
[verbose] pd: gui_iemgui_label_new "x2795b20","x275e9b0",5,9,"#9c9c9c","0","DejaVu Sans Mono","normal",16
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d0a90","atom",401,9,1
[verbose] pd: gui_text_set "x2795b20",".x2795b20.t27d0a90","0"
[verbose] pd: gui_atom_draw_border "x2795b20",".x2795b20.t27d0a90",32,13
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d0a90",".x2795b20.t27d0a90",401,19,408,22,401,9,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d0a90",".x2795b20.t27d0a90",401,9,408,12,401,9,"i",0,0,0
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d0a90","atom",0,2,10,"0",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d0e80","atom",25,58,1
[verbose] pd: gui_text_set "x2795b20",".x2795b20.t27d0e80","0  "
[verbose] pd: gui_atom_draw_border "x2795b20",".x2795b20.t27d0e80",22,14
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d0e80",".x2795b20.t27d0e80",25,69,32,72,25,58,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d0e80",".x2795b20.t27d0e80",25,58,32,61,25,58,"i",0,0,0
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d0e80","atom",0,2,10,"0  ",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d1270","atom",25,96,1
[verbose] pd: gui_text_set "x2795b20",".x2795b20.t27d1270","0  "
[verbose] pd: gui_atom_draw_border "x2795b20",".x2795b20.t27d1270",22,14
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d1270",".x2795b20.t27d1270",25,107,32,110,25,96,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d1270",".x2795b20.t27d1270",25,96,32,99,25,96,"i",0,0,0
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d1270","atom",0,2,10,"0  ",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d1660","msg",469,76,1
[verbose] pd: gui_message_draw_border "x2795b20",".x2795b20.t27d1660",40,14
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d1660",".x2795b20.t27d1660",469,87,476,90,469,76,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d1660",".x2795b20.t27d1660",469,76,476,79,469,76,"i",0,0,0
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d1660","msg",0,2,10,"set 45",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d1a70","atom",469,104,1
[verbose] pd: gui_text_set "x2795b20",".x2795b20.t27d1a70","0  "
[verbose] pd: gui_atom_draw_border "x2795b20",".x2795b20.t27d1a70",22,14
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d1a70",".x2795b20.t27d1a70",469,115,476,118,469,104,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d1a70",".x2795b20.t27d1a70",469,104,476,107,469,104,"i",0,0,0
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d1a70","atom",0,2,10,"0  ",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d1e60","comment",98,239,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d1e60","text",0,2,10,"bang",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d2260","comment",98,259,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d2260","text",0,2,10,"float",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d2670","comment",98,289,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d2670","text",0,2,10,"list",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d2a70","comment",98,309,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d2a70","text",0,2,10,"symbol",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d2e80","comment",98,339,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d2e80","text",0,2,10,"set",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d3280","obj",102,571,1
[verbose] pd: gui_text_draw_border "x2795b20",".x2795b20.t27d3280","none",0,102,571,250,585
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d3280","obj",0,2,10,"pd Number_box_properties",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d3720","obj",4,597,1
[verbose] pd: gui_text_draw_border "x2795b20",".x2795b20.t27d3720","none",1,4,597,350,611
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d3720","obj",0,2,10,"pddp/pddplink all_about_help_patches.pd -text Usage Guide",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d3cd0","atom",25,138,1
[verbose] pd: gui_text_set "x2795b20",".x2795b20.t27d3cd0","symbol"
[verbose] pd: gui_atom_draw_border "x2795b20",".x2795b20.t27d3cd0",62,13
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d3cd0",".x2795b20.t27d3cd0",25,148,32,151,25,138,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d3cd0",".x2795b20.t27d3cd0",25,138,32,141,25,138,"i",0,0,0
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d3cd0","atom",0,2,10,"symbol",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d40e0","comment",11,23,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d40e0","text",0,2,10,"atom (number box and symbol box)",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d45c0","atom",481,9,1
[verbose] pd: gui_text_set "x2795b20",".x2795b20.t27d45c0","symbol"
[verbose] pd: gui_atom_draw_border "x2795b20",".x2795b20.t27d45c0",62,13
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d45c0",".x2795b20.t27d45c0",481,19,488,22,481,9,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d45c0",".x2795b20.t27d45c0",481,9,488,12,481,9,"i",0,0,0
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d45c0","atom",0,2,10,"symbol",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d49d0","comment",445,9,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d49d0","text",0,2,10,"and",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d4dd0","atom",25,174,1
[verbose] pd: gui_text_set "x2795b20",".x2795b20.t27d4dd0","symbol"
[verbose] pd: gui_atom_draw_border "x2795b20",".x2795b20.t27d4dd0",62,13
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d4dd0",".x2795b20.t27d4dd0",25,184,32,187,25,174,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d4dd0",".x2795b20.t27d4dd0",25,174,32,177,25,174,"i",0,0,0
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d4dd0","atom",0,2,10,"symbol",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d51e0","comment",61,52,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d51e0","text",0,2,10,"A number box allows you to display a number or enter a
number using the mouse and keyboard. When a number arrives
at the number box's inlet, it is displayed and sent to the
outlet. You can click on a number box and drag upward or
downward to change the value continuously.",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d5e40","comment",97,134,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d5e40","text",0,2,10,"A symbol box allows you to display a single symbol-atom or
enter one using the mouse and keyboard. Unlike a number box
you cannot change the value by clicking and dragging.",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d6780","comment",96,174,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d6780","text",0,2,10,"The symbol box is called \"Symbol\" in the \"Put\" menu.",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d6d00","comment",168,239,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d6d00","text",0,2,10,"- sends the current value to the outlet.",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d7220","msg",469,156,1
[verbose] pd: gui_message_draw_border "x2795b20",".x2795b20.t27d7220",46,14
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d7220",".x2795b20.t27d7220",469,167,476,170,469,156,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d7220",".x2795b20.t27d7220",469,156,476,159,469,156,"i",0,0,0
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d7220","msg",0,2,10,"set foo",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d7640","atom",469,180,1
[verbose] pd: gui_text_set "x2795b20",".x2795b20.t27d7640","symbol"
[verbose] pd: gui_atom_draw_border "x2795b20",".x2795b20.t27d7640",62,13
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d7640",".x2795b20.t27d7640",469,190,476,193,469,180,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27d7640",".x2795b20.t27d7640",469,180,476,183,469,180,"i",0,0,0
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d7640","atom",0,2,10,"symbol",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d7a50","comment",168,309,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d7a50","text",0,2,10,"- symbol box only: an incoming symbol is displayed and
output. (Number box will display and output zero.)",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d8180","comment",168,259,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d8180","text",0,2,10,"- number box only: sets the current value and outputs it.
(Symbol box will display 'float' and output 'symbol float'.)",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d8910","comment",168,289,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d8910","text",0,2,10,"- a list will be truncated to the first item.",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d8e60","comment",168,339,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d8e60","text",0,2,10,"- displays the incoming value without outputting it (e.g.,
\"set 23\" will cause a number box to display 23). Sending a
set message without a value does not change the current
value of a number box or symbol box.",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d98d0","comment",168,404,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d98d0","text",0,2,10,"- number box only: outputs a float for each message it
receives (except set).",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27d9f20","comment",98,434,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27d9f20","text",0,2,10,"symbol",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27da330","comment",168,434,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27da330","text",0,2,10,"- symbol box only: outputs a symbol message for each message
it receives (except set).",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27da9c0","comment",100,537,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27da9c0","text",0,2,10,"Control-clicking (or command-clicking on a mac) toggles the
value between 0 and the last nonzero value.",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27db0e0","atom",59,545,1
[verbose] pd: gui_text_set "x2795b20",".x2795b20.t27db0e0","0"
[verbose] pd: gui_atom_draw_border "x2795b20",".x2795b20.t27db0e0",32,13
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27db0e0",".x2795b20.t27db0e0",59,555,66,558,59,545,"o",0,0,0
[verbose] pd: gui_gobj_draw_io "x2795b20",".x2795b20.t27db0e0",".x2795b20.t27db0e0",59,545,66,548,59,545,"i",0,0,0
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27db0e0","atom",0,2,10,"0",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27db4d0","comment",100,503,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27db4d0","text",0,2,10,"You can shift-click a number box and drag to change the
number by hundredths instead of ones.",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27dbba0","comment",61,118,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27dbba0","text",0,2,10,"The number box is called \"Number\" in the \"Put\" menu.",5
[verbose] pd: gui_gobj_new "x2795b20",".x2795b20.t27dc120","comment",67,197,1
[verbose] pd: gui_text_new "x2795b20",".x2795b20.t27dc120","text",0,2,10,"To enter data simply click a number box or symbol box and
begin typing. Then click \"Enter\" to finish and output it.",5
[verbose] pd: gui_canvas_line "x2795b20","l27df6c0","control",3,0,3,10,15,48,28,86,28,96
[verbose] pd: gui_canvas_line "x2795b20","l27df790","control",3,0,3,10,237,52,472,94,472,104
[verbose] pd: gui_canvas_line "x2795b20","l27df860","control",3,0,3,10,15,87,28,164,28,174
[verbose] pd: gui_canvas_line "x2795b20","l27df930","control",3,0,3,10,237,90,472,170,472,180
[verbose] pd: gui_canvas_get_scroll "x2795b20"
[verbose] pd: gui_set_toplevel_window_list "dummy",["gatom-help.pd","x2795b20","main.pd","x268b500"]
[verbose] pd: gui_pd_dsp 1
[verbose] pd: gui_canvas_cursor "x2795b20","cursor_editmode_nothing"
[verbose] pd: gui_canvas_set_editmode "x2795b20",1
[verbose] pd: gui_canvas_cursor "x268b500","cursor_editmode_nothing"
[verbose] pd: gui_canvas_draw_selection "x268b500",23,31,23,31
[verbose] pd: gui_canvas_hide_selection "x268b500"
[verbose] pd: gui_canvas_cursor "x268b500","cursor_runmode_nothing"
[verbose] pd: gui_canvas_set_editmode "x268b500",0
[verbose] pd: gui_text_set "x268b500","x270f100","1"
[verbose] pd: gui_text_set "x268b500","x270f100","2"
[verbose] pd: gui_text_set "x268b500","x270f100","3"
[verbose] pd: gui_text_set "x268b500","x270f100","4"
[verbose] pd: gui_text_set "x268b500","x270f100","5"
[verbose] pd: gui_text_set "x268b500","x270f100","6"
[verbose] pd: gui_text_set "x268b500","x270f100","7"
[verbose] pd: gui_text_set "x268b500","x270f100","8"
[verbose] pd: gui_text_set "x268b500","x270f100","9"
[verbose] pd: gui_text_set "x268b500","x270f100","10"
[verbose] pd: gui_text_set "x268b500","x270f100","11"
[verbose] pd: gui_text_set "x268b500","x270f100","12"
[verbose] pd: gui_text_set "x268b500","x270f100","12"

*/
