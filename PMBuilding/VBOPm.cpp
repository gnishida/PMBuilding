/************************************************************************************************
 *		Procedural City Generation
 *		@author igarciad
 ************************************************************************************************/

#include "VBOPm.h"
#include "Polygon3D.h"

#include <qdir.h>
#include <QStringList>
#include <QTime>

#include "VBOGeoBuilding.h"
#include "Polygon3D.h"
#include "Util.h"

// LC
bool VBOPm::initializedLC=false;

/**
 * テクスチャ画像の読み込み
 */
void VBOPm::initLC(){
	QString pathName="../data/textures/LC";

	printf("-->Initialized LC\n");
	initializedLC=true;
}

bool VBOPm::generateBuildings(VBORenderManager& rendManager) {
	rendManager.removeStaticGeometry("3d_building");
	rendManager.removeStaticGeometry("3d_building_fac");
			
	Building building;
	building.bldType = 1;
	building.buildingFootprint.push_back(QVector3D(-10, -10, 0));
	building.buildingFootprint.push_back(QVector3D(10, -10, 0));
	building.buildingFootprint.push_back(QVector3D(10, 10, 0));
	building.buildingFootprint.push_back(QVector3D(-10, 10, 0));
	building.numStories = 10;

	VBOGeoBuilding::generateBuilding(rendManager, building);

	printf("Building generation is done.\n");

	return true;
}

