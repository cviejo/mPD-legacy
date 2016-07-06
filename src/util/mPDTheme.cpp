#include "mPDTheme.h"
#include "../Globals.h"


//--------------------------------------------------------------
void mPDTheme::load(string aTheme){

	this->loadFile(aTheme);

	this->font.file = this->getString("theme:font:file");

	this->grid.cell.width  = (int)this->getFloat("theme:grid:cell:width");
	this->grid.cell.height = (int)this->getFloat("theme:grid:cell:height");
	this->grid.color.front.set(this->getColor("theme:grid:color:front"));

	this->button.width   = this->getScaledValue("theme:button:width");
	this->button.height  = this->getScaledValue("theme:button:height");
	this->button.margin  = this->getScaledValue("theme:button:margin");
	this->button.padding = this->getScaledValue("theme:button:padding");
	this->button.color.background.set(this->getColor("theme:button:color:background"));
	this->button.color.pressed.set(this->getColor("theme:button:color:pressed"));

	this->tab.collapsedWidth = this->getScaledValue("theme:tab:collapsedWidth");
	this->tab.color.background.set(this->getColor("theme:tab:color:background"));

	this->frame.color.background.set(this->getColor("theme:frame:color:background"));

	this->console.font.height = this->getScaledValue("theme:console:font:height");
	this->console.color.front.set(this->getColor("theme:console:color:front"));

	this->canvas.color.background.set(this->getColor("theme:canvas:color:background"));
	this->canvas.color.grid.set(this->getColor("theme:canvas:color:grid"));

	this->node.font.height  = (int)this->getFloat("theme:node:font:height");
	this->node.color.background.set(this->getColor("theme:node:color:background"));
	this->node.color.border.set(this->getColor("theme:node:color:border"));
	this->node.color.front.set(this->getColor("theme:node:color:front"));
	this->node.color.disabled.set(this->getColor("theme:node:color:disabled"));
	this->node.color.invalid.set(this->getColor("theme:node:color:invalid"));
	this->node.io.height  = (int)this->getFloat("theme:node:io:height");
	this->node.io.color.signal.set(this->getColor("theme:node:color:signal"));
	this->node.io.color.control.set(this->getColor("theme:node:color:control"));

	this->control.color.background.set(this->getColor("theme:control:color:background"));
	this->control.color.border.set(this->getColor("theme:control:color:border"));
	this->control.color.front.set(this->getColor("theme:control:color:front"));
	this->control.color.disabled.set(this->getColor("theme:control:color:disabled"));
	this->control.color.control.set(this->getColor("theme:control:color:control"));

	this->connection.color.signal.set(this->getColor("theme:connection:color:signal"));
	this->connection.color.control.set(this->getColor("theme:connection:color:control"));

	this->indexScroller.width = this->getScaledValue("theme:index-scroller:width");
	this->indexScroller.color.background.set(this->getColor("theme:index-scroller:color:background"));
	this->indexScroller.color.selection.set(this->getColor("theme:index-scroller:color:selection"));
	this->indexScroller.color.front.set(this->getColor("theme:index-scroller:color:front"));

	this->objectScroller.width   = this->getScaledValue("theme:object-scroller:width");
	this->objectScroller.padding = this->getScaledValue("theme:object-scroller:padding");
	this->objectScroller.color.background.set(this->getColor("theme:object-scroller:color:background"));
	this->objectScroller.color.front.set(this->getColor("theme:object-scroller:color:front"));

	this->objectScroller.item.height  = this->getScaledValue("theme:object-scroller:item:height");
	this->objectScroller.item.padding = this->getScaledValue("theme:object-scroller:item:padding");
	this->objectScroller.item.color.background.set(this->getColor("theme:object-scroller:item:color:background"));
	this->objectScroller.item.color.selection.set(this->getColor("theme:object-scroller:item:color:selection"));
	this->objectScroller.item.color.front.set(this->getColor("theme:object-scroller:item:color:front"));

	this->objectScroller.header.height = this->getScaledValue("theme:object-scroller:header:height");
	this->objectScroller.header.color.background.set(this->getColor("theme:object-scroller:header:color:background"));
	this->objectScroller.header.color.front.set(this->getColor("theme:object-scroller:header:color:front"));
}


//--------------------------------------------------------------
int mPDTheme::getScaledValue(string aPath){

	return (int)(this->getFloat(aPath) * Globals::DPI);
}
