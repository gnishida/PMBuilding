#pragma once

#include <QString>
#include <vector>
#include "VBORenderManager.h"
#include "Building.h"

class PMBuildingHouse {
public:
	static std::vector<QString> houseTex;
	static bool initialized;

public:
	static void initTex();
	static void generate(VBORenderManager& rendManager, Building& building);
};

