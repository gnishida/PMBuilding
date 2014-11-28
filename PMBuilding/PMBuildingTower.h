/************************************************************************************************
 *		Procedural City Generation: Buildings geometry
 *		@author igarciad
 ************************************************************************************************/

#pragma once

#include "Building.h"
#include "VBORenderManager.h"

class PMBuildingTower {
public:
	static bool initialized;
	static std::vector<QString> facadeTex;
	static std::vector<QVector3D> facadeScale;
	static std::vector<QString> windowTex;
	static std::vector<QString> roofTex;

public:
	static void addWindow(VBORenderManager& rendManager, int windowTexId, QVector3D initPoint,QVector3D dirR,QVector3D dirUp,float width,float height);
	static void addColumnGeometry(VBORenderManager& rendManager, std::vector<QVector3D>& columnContour, int randomFacade, int windowTexId, float uS, float vS, float height, int numFloors);
	static void calculateColumnContour(std::vector<QVector3D>& currentContour, std::vector<QVector3D>& columnContour);
	static void generate(VBORenderManager& rendManager, Building& building);
	static void initTex();
};
