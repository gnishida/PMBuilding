#pragma once

#include <QString>
#include <vector>
#include "VBORenderManager.h"
#include "Building.h"

class PMBuildingFactory {
public:
	static std::vector<QString> textures;
	static bool initialized;

public:
	static void initialize();
	static void generate(VBORenderManager& rendManager, Building& building);
};

