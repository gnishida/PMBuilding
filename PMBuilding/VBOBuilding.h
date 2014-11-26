#pragma once

#include <QSettings>
#include "Polygon3D.h"

class Building {
public:
	Polygon3D buildingFootprint;
	int bldType;
	int numStories;

public:
	Building() : bldType(-1), numStories(-1) {}
};
