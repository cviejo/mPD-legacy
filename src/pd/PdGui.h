#pragma once


#include "PdBase.hpp"
#include "PdGuiTypes.h"


using namespace pd;


class PdGui : public PdBase, protected PdReceiver, protected PdMidiReceiver {

	public:

		static PdGui& instance();

		bool init(const int inputChannels,
		          const int outputChannels,
		          const int sampleRate,
		          const int ticksPerBuffer=32,
		          bool queued=false);

		void              clear();
		void              startAudio();
		void              stopAudio();
		PdCanvas*         openPatch(const string& aPath); // naming/types are a bit off on these two
		void              closePatch(PdCanvas& aPatch);
		void              canvasPressed (int x, int y);
		void              canvasDragged (int x, int y);
		void              canvasReleased(int x, int y);
		void              canvasDelete();
		void              canvasCopy();
		void              canvasPaste();
		void              canvasUndo();
		void              guiMessage(string aMessage);
		vector<PdCanvas*> getCanvases();
		PdCanvas*         getCanvas(string aId);
		PdNode*           getNode(string aCanvasId, string aNodeId);

		virtual void      audioIn (float * input,  int bufferSize, int channelCount);
		virtual void      audioOut(float * output, int bufferSize, int channelCount);


	private:

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

		string unquote(string& str);
		string            getPatchId(Patch& patch);
		void              evaluateBuffer(string& str);
};

