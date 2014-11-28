#pragma once

#include <QString>
#include <vector>
#include "VBORenderManager.h"
#include "Building.h"

class PMBuildingFactory {
public:
	static std::vector<QString> textures;
	static bool initialized;
	static int NUM_SUBTYPE;
	static int NUM_SUBTYPE_1;
	static int NUM_SUBTYPE_2;

public:
	static void initialize();
	static void generate(VBORenderManager& rendManager, Building& building);
};

