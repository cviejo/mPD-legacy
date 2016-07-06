#include "Tab.h"
#include "IndexScroller.h"
#include "ObjectScroller.h"


//--------------------------------------------------------------
class ObjectTab : public Tab {

	public:

		ObjectTab();

		virtual void update();

		void setContent(vector<string> aItems);

	private:

		int            _margin;
		ObjectScroller _objectScroller;
		IndexScroller  _indexScroller;
};