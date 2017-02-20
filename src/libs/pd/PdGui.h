#pragma once


#include "PdBase.hpp"
#include "PdGuiTypes.h"
#include "m_pd.h"


using namespace pd;


class PdGui : public PdBase, protected PdReceiver, protected PdMidiReceiver {

	public:

		bool              updateNeeded = true;

		static PdGui&     instance(); // singleton

		bool              init(const int in, const int out, const int sampleRate, const int ticks=32,bool queued=false);
		void              clear();
		void              startAudio();
		void              stopAudio();
		PdCanvas*         openPatch(const string& aPath); // naming/types are a bit off on these two
		void              closePatch(PdCanvas& aPatch);
		void              pdsend(string& cmd, bool aLock = true);
		void              guiMessage(string aMessage);
		vector<PdCanvas*> getCanvases();
		PdCanvas*         getCanvas(string aId);
		PdNode*           getNode(string aCanvasId, string aNodeId);
		vector<string>    getNodeNames();

		virtual void      audioIn (float * input,  int bufferSize, int channelCount);
		virtual void      audioOut(float * output, int bufferSize, int channelCount);


	private:

		// singleton
		PdGui();
		virtual ~PdGui();
		void operator = (PdGui& from) {}

		bool              _computing   = false;
		float*            _inputBuffer = NULL;
		vector<PdCanvas*> _canvases;
		int               _ticks;
		int               _bufferSize;
		int               _sampleRate;
		int               _outputChannels;
		int               _inputChannels;
		vector<string>    _classNames;

		void              displaceObject(PdNode* aNode, ofPoint aOffset);
		string            unquote(string& str);
		string            getPatchId(Patch& patch);
		// PdNode*           getNewNode(PdGuiMessage& aGuiMsg);

		t_canvas*         getPdCanvas(string aId);
};

