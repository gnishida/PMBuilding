#pragma once

#include <QString>
#include <vector>
#include "VBORenderManager.h"
#include "Building.h"

class PMBuildingRB {
public:
	static std::vector<QString> textures;
	static bool initialized;
	static int NUM_SUBTYPE;

public:
	static void initialize();
	static void generate(VBORenderManager& rendManager, const QString& geoName, Building& building);
	static void generateType0(VBORenderManager& rendManager, const QString& geoName, Building& building);
	static void generateType1(VBORenderManager& rendManager, const QString& geoName, Building& building);
};

