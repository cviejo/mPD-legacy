#pragma once


#include "XmlWrapper.h"


//--------------------------------------------------------------
class mPDTheme : public XmlWrapper {

	public:

		struct { // font
			string file;
		} font; 

		struct { // grid
			struct { // grid-cell
				int width;
				int height;
			} cell;
			struct {
				ofColor front;
			} color; 
		} grid; 

		struct { // frame
			struct {
				ofColor background;
			} color; 
		} frame; 

		struct { // tab
			int collapsedWidth;
			struct {
				ofColor background;
			} color; 
		} tab; 

		struct {
			struct {
				int height;
			} font;
			struct {
				ofColor front;
			} color;
		} console;

		struct { // canvas
			struct {
				ofColor background;
				ofColor grid;
			} color;
		} canvas;

		struct { // node
			struct {
				int height;
			} font;
			struct {
				ofColor background;
				ofColor border;
				ofColor front;
				ofColor disabled;
				ofColor invalid;
			} color; 
			struct {
				int height;
				struct {
					ofColor signal;
					ofColor control;
				} color;
			} io;
		} node; 

		struct { // control
			struct {
				ofColor background;
				ofColor border;
				ofColor front;
				ofColor disabled;
				ofColor control;
			} color; 
		} control; 

		struct { // connection
			struct {
				ofColor signal;
				ofColor control;
			} color; 
		} connection; 

		struct { // object-scroller
			int width;
			int padding;
			struct {
				ofColor background;
				ofColor front;
			} color; 
			struct { // object-scroller > item
				int height;
				int padding;
				struct {
					ofColor background;
					ofColor selection;
					ofColor front;
				} color; 
			} item;
			struct { // object-scroller > header
				int height;
				struct {
					ofColor background;
					ofColor front;
				} color; 
			} header;
		} objectScroller; 
		struct { // index-scroller
			int width;
			struct {
				ofColor background;
				ofColor selection;
				ofColor front;
			} color; 
		} indexScroller;
		struct { // button
			int width;
			int height;
			int margin;
			int padding;
			struct {
				ofColor background;
				ofColor pressed;
			} color; 
		} button;

		void load          (string aPath);
		int  getScaledValue(string aPath);
};
