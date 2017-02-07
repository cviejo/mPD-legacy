#pragma once

#include "ofPath.h"


class Svg {

	public:

		float width, height;
		vector <ofPath> paths;

		void loadData(string aFormat, string aData);
		void setupDiagram(struct svgtiny_diagram * diagram);
		void setupShape(struct svgtiny_shape * shape, ofPath & path);
		void draw();

		const vector <ofPath> & getPaths() const;
};

