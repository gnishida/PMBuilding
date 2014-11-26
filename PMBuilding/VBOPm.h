/************************************************************************************************
 *		Procedural City Generation
 *		@author igarciad
 ************************************************************************************************/

#pragma once

#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>

#include "VBORenderManager.h"
#include "VBOBuilding.h"

class VBORenderManager;

class VBOPm {
public:
	static bool initializedLC;
	static void initLC();

	static bool generateBuildings(VBORenderManager& rendManager);
};


