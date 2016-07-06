#include "GUIElement.h"
#include "Tab.h"
#include "ButtonGroup.h"


//--------------------------------------------------------------
class MenuTab : public Tab {

	public:

		MenuTab();

		virtual void update();

	private:

		ButtonGroup _zoomControls;
		ButtonGroup _editControls;
};
