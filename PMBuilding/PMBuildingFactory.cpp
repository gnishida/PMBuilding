#include "PMBuildingFactory.h"

bool PMBuildingFactory::initialized = false;
std::vector<QString> PMBuildingFactory::textures;

void PMBuildingFactory::initialize() {
	if (initialized) return;

	textures.push_back("../data/textures/LC/house/wall.jpg");
	textures.push_back("../data/textures/LC/house/door.jpg");
	textures.push_back("../data/textures/LC/house/garage.jpg");
	textures.push_back("../data/textures/LC/house/roof.jpg");
	textures.push_back("../data/textures/LC/house/window.jpg");
	textures.push_back("../data/textures/LC/house/window2.jpg");

	initialized = true;
}

void PMBuildingFactory::generate(VBORenderManager& rendManager, Building& building) {
}